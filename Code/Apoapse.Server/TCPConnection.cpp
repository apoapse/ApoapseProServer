#include "stdafx.h"
#include "Common.h"
#include "TCPConnection.h"

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

void TCPConnection::Send(const boost::asio::const_buffer& inputBuffer)
{
	auto handler = boost::bind(&TCPConnection::HandleWriteAsync, shared_from_this(), boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred);
	boost::asio::async_write(m_socket, boost::asio::buffer(inputBuffer), handler);
}

void TCPConnection::HandleConnectAsync(const boost::system::error_code& error)
{
	if (!error)
		m_isConnected = true;

	if (this->OnConnectedToServer(error))
	{
		LOG_DEBUG_ONLY(Format("%1% connected to %2%, port %3%", __FUNCTION__, GetEndpoint().address(), GetEndpoint().port()));
		ListenIncomingNewMessages();
	}
	else
		Close();
}

void TCPConnection::HandleAcceptedAsync(const boost::system::error_code& error)
{
	LOG_DEBUG_ONLY(Format("%1% accepeted by server %2%, port %3%", __FUNCTION__, GetEndpoint().address(), GetEndpoint().port()));
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
	ASSERT(bytesTransferred == HEADER_LENGTH);	// #TODO

	LOG_DEBUG_ONLY(Format("%1% received %4% bytes from %2%, port %3%", __FUNCTION__, GetEndpoint().address(), GetEndpoint().port(), bytesTransferred));

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
	m_currentNetMessage = std::make_shared<NetMessage>(expectedContentSize);

	ListenIncomingMessageContent();
}

void TCPConnection::ReadReceivedContent(const boost::system::error_code& error, size_t bytesTransferred)
{
	if (bytesTransferred == 0)
		return;

	LOG_DEBUG_ONLY(Format("%1% received %4% bytes from %2%, port %3% (%5% bytes left)", __FUNCTION__, GetEndpoint().address(), GetEndpoint().port(), bytesTransferred, -1));//TODO

	m_currentNetMessage->AppendData<byte, SOCKET_READ_BUFFER_SIZE>(m_readContentBuffer, bytesTransferred);

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

void TCPConnection::HandleWriteAsync(const boost::system::error_code& error, size_t bytesTransferred)
{
	LOG_DEBUG_ONLY(Format("%1% send %4% bytes to %2%, port %3%", __FUNCTION__, GetEndpoint().address(), GetEndpoint().port(), bytesTransferred));

	if (!this->OnSentPacket(error, bytesTransferred))
		Close();
}