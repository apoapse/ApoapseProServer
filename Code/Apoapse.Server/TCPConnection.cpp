#include "stdafx.h"
#include "Common.h"
#include "TCPConnection.h"
#include "UnicodeUtils.h"

TCPConnection::TCPConnection(boost::asio::io_service& io_service)
	: m_socket(io_service),
	m_writeStrand(io_service)
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
	if (error && error != boost::asio::error::not_found)
	{
		OnReceivedError(error);
		return;
	}

	handler(bytesTransferred);
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

void TCPConnection::ReadSome(boost::asio::streambuf& streambuf, size_t length, std::function<void(size_t)> externalHandler)
{
	auto handler = boost::bind(&TCPConnection::HandleReadInternal, shared_from_this(), externalHandler, boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred);

	boost::asio::async_read(GetSocket(), streambuf, boost::asio::transfer_exactly(length), handler);
}
