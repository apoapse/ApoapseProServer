#include "stdafx.h"
#include "Common.h"
#include "TCPConnection.h"
#include "UnicodeUtils.h"

TCPConnection::TCPConnection(boost::asio::io_service& io_service)
	: m_socket(io_service),
	m_writeStrand(io_service)/*,
	m_readStreamBuffer(SOCKET_READ_BUFFER_SIZE)*/
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
		ASSERT_MSG(false, "TCPConnection::GetEndpoint() is probably called too early");
		return boost::asio::ip::tcp::endpoint();
	}
}

void TCPConnection::HandleConnectAsync(const boost::system::error_code& error)
{
	LOG << "Connected to " << GetEndpoint().address() << ", port " << GetEndpoint().port();

	if (error)
		OnReceivedErrorInternal(error);
	else
	{
		this->OnConnectedToServer();
		m_isConnected = true;
	}
}

void TCPConnection::HandleAcceptedAsync(const boost::system::error_code& error)
{
	LOG_DEBUG_ONLY("TCPConnection Accepeted " << " bytes to " << GetEndpoint().address() << ", port " << GetEndpoint().port());
	HandleConnectAsync(error);
}

void TCPConnection::OnReceivedErrorInternal(const boost::system::error_code& error)
{
	if (!this->OnReceivedError(error))
		Close();
}

void TCPConnection::HandleReadInternal(const std::function<void(size_t)>& handler, const boost::system::error_code& error, size_t bytesTransferred)
{
	if (error)
	{
		OnReceivedError(error);
		return;
	}

	handler(bytesTransferred);
}

void TCPConnection::ListenForCommandName()
{
	ASSERT(IsConnected());


/*

	auto handler = boost::bind(&TCPConnection::ReadHeader, shared_from_this(), boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred);
	boost::asio::async_read(m_socket, boost::asio::buffer(m_readHeaderBuffer, HEADER_LENGTH), boost::asio::transfer_at_least(HEADER_LENGTH), handler);*/
}

void TCPConnection::ListenForInlineCommandContent()
{
/*
	auto handler = boost::bind(&TCPConnection::ReadReceivedContent, shared_from_this(), boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred);
	m_socket.async_read_some(boost::asio::buffer(m_readContentBuffer), handler);*/
}

void TCPConnection::QueueSend(const std::vector<byte> data)
{
/*
	const bool writeInProgress = !m_sendQueue.empty();
	m_sendQueue.push_back(netMessage);

	if (!writeInProgress)
		InternalSend();*/
}

void TCPConnection::InternalSend()
{
	/*auto handler = boost::bind(&TCPConnection::HandleWriteAsync, shared_from_this(), boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred);
	boost::asio::async_write(m_socket, boost::asio::buffer(m_sendQueue.front()->GetRawData()), handler);*/
}

void TCPConnection::HandleWriteAsync(const boost::system::error_code& error, size_t bytesTransferred)
{
/*
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
		OnReceivedErrorInternal(error);*/
}