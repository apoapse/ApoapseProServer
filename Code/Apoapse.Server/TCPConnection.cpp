#include "stdafx.h"
#include "Common.h"
#include "TCPConnection.h"
#include "UTF8.h"

TCPConnection::TCPConnection(boost::asio::io_service& io_service) : m_socket(io_service), m_writeStrand(io_service)
{
}

void TCPConnection::Connect(const std::string& adress, const UInt16 port)
{
	ASSERT(!IsConnected());
	auto destination = boostTCP::endpoint(boost::asio::ip::address::from_string(adress), port);

	m_socket.async_connect(destination, boost::bind(&TCPConnection::HandleConnectAsync, shared_from_this(), boost::asio::placeholders::error));
}

bool TCPConnection::IsConnected() const
{
	return m_isConnected;
}

void TCPConnection::Close()
{
	m_isConnected = false;
	m_socket.close();
}

boost::asio::ip::tcp::endpoint TCPConnection::GetEndpoint() const
{
	try
	{
		return m_socket.remote_endpoint();
	}
	catch (...)
	{
		ASSERT_MSG(false, "TCPConnection::GetEndpoint() is called too early");
		return boost::asio::ip::tcp::endpoint();
	}
}

void TCPConnection::HandleConnectAsync(const boost::system::error_code& error)
{
	LOG << "Connected to " << GetEndpoint().address() << ", port " << GetEndpoint().port();

	if (!error)
		m_isConnected = true;

	if (this->OnConnectedToServer(error))
	{
		ListenIncomingNewMessages();
	}
	else
		Close();
}

void TCPConnection::HandleAcceptedAsync(const boost::system::error_code& error)
{
	LOG_DEBUG_ONLY("Accepeted " << " bytes to " << GetEndpoint().address() << ", port " << GetEndpoint().port());
	HandleConnectAsync(error);
}

void TCPConnection::ListenIncomingNewMessages()
{
	ASSERT(IsConnected());

	auto handler = boost::bind(&TCPConnection::ReadHeader, shared_from_this(), boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred);
	boost::asio::async_read(m_socket, boost::asio::buffer(m_readHeaderBuffer, HEADER_LENGTH), boost::asio::transfer_at_least(HEADER_LENGTH), handler);
}

void TCPConnection::ListenIncomingMessageContent()
{
	auto handler = boost::bind(&TCPConnection::ReadReceivedContent, shared_from_this(), boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred);
	m_socket.async_read_some(boost::asio::buffer(m_readContentBuffer), handler);
}

void TCPConnection::ReadHeader(const boost::system::error_code& error, size_t bytesTransferred)
{
	if (bytesTransferred == 0)
	{
		Close();
		return;
	}
	ASSERT(bytesTransferred == HEADER_LENGTH);	// #TODO

	LOG_DEBUG_ONLY("received " << bytesTransferred << " bytes to " << GetEndpoint().address() << ", port " << GetEndpoint().port());

	if (error)
	{
		if (!this->OnReadError(error))
		{
			Close();
			return;
		}
	}

	// Read header
	UInt32 expectedContentSize = ByteConverter::ToUInt32(m_readHeaderBuffer);
	m_currentNetMessage = std::make_shared<NetMessage>(expectedContentSize, false);

	ListenIncomingMessageContent();
}

void TCPConnection::ReadReceivedContent(const boost::system::error_code& /*error*/, size_t bytesTransferred)
{
	if (bytesTransferred == 0)
		return;

	LOG_DEBUG_ONLY("received " << bytesTransferred << " bytes to " << GetEndpoint().address() << ", port " << GetEndpoint().port());	// #TODO

	m_currentNetMessage->AppendData<SOCKET_READ_BUFFER_SIZE>(m_readContentBuffer, bytesTransferred);

	if (m_currentNetMessage->IsComplete())
	{
		if (!OnReceivedPacket(m_currentNetMessage))
			Close();
		else
			ListenIncomingNewMessages();
	}
	else
	{
		ListenIncomingMessageContent();
	}
}


void TCPConnection::Send(const std::string& msg)
{
	// Create header
	std::array<byte, HEADER_LENGTH> header;
	const UInt32 contentSize = (UInt32)msg.length();
	ASSERT(contentSize > 0);

	ByteConverter::FromUInt32(header.data(), contentSize);
	//UInt32 test = ByteConverter::ToUInt32(header.data());

	auto netMessage = std::make_shared<NetMessage>(contentSize + HEADER_LENGTH, true);
	netMessage->AppendData<HEADER_LENGTH>(header, HEADER_LENGTH);
	netMessage->AppendStr(msg);

	m_socket.get_io_service().post(m_writeStrand.wrap([netMessage, currentObj=shared_from_this()] {	// #TODO test in order to make sure this is completely thread safe
		currentObj->QueueSendNetMessage(netMessage);
	}));
}

void TCPConnection::QueueSendNetMessage(const std::shared_ptr<NetMessage> netMessage)
{
	const bool writeInProgress = !m_sendQueue.empty();
	m_sendQueue.push_back(netMessage);

	if (!writeInProgress)
		InternalSend();
}

void TCPConnection::InternalSend()
{
	auto handler = boost::bind(&TCPConnection::HandleWriteAsync, shared_from_this(), boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred);
	boost::asio::async_write(m_socket, boost::asio::buffer(m_sendQueue.front()->GetRawData()), handler);
}

void TCPConnection::HandleWriteAsync(const boost::system::error_code& error, size_t bytesTransferred)
{
	ASSERT(m_sendQueue.front()->GetRawData().size() == bytesTransferred);

	if (!error)
	{
		LOG_DEBUG_ONLY("send successfully " << bytesTransferred << " bytes to " << GetEndpoint().address() << ", port " << GetEndpoint().port());

		if (this->OnSentPacket(m_sendQueue.front(), bytesTransferred))
			m_sendQueue.pop_front();
		else
			Close();
	}
	else
		OnReadError(error);
}