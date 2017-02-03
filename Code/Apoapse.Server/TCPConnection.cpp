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
	catch (const std::exception&)
	{
		ASSERT_MSG(false, "TCPConnection::GetEndpoint() is probably called too early");
		return boost::asio::ip::tcp::endpoint();
	}
}

void TCPConnection::HandleConnectAsync(const boost::system::error_code& error)
{
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
	LOG_DEBUG_ONLY("TCPConnection accepeted from " << GetEndpoint());
	HandleConnectAsync(error);
}

void TCPConnection::OnReceivedErrorInternal(const boost::system::error_code& error)
{
	if (!this->OnReceivedError(error))
		Close();
}

void TCPConnection::HandleReadInternal(const std::function<void(size_t)>& handler, const boost::system::error_code& error, size_t bytesTransferred)
{
	if (!IsConnected())
		return;

	if (error && error != boost::asio::error::not_found)
	{
		OnReceivedErrorInternal(error);
		return;
	}

	handler(bytesTransferred);
}

void TCPConnection::Send(const std::vector<byte>& bytes)
{

	const bool writeInProgress = !m_sendQueue.empty();
	m_sendQueue.push_back(bytes);

	if (!writeInProgress)
		InternalSend();
}

void TCPConnection::Send(const std::string& str)
{
	const bool writeInProgress = !m_sendQueue.empty();
	m_sendQueue.push_back(str);

	if (!writeInProgress)
	InternalSend();
}

void TCPConnection::InternalSend()
{
	const auto& item = m_sendQueue.front();

	if (item.type() == typeid(std::string))
	{
		auto handler = boost::bind(&TCPConnection::HandleWriteAsync, shared_from_this(), boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred);
		boost::asio::async_write(m_socket, boost::asio::buffer(boost::get<std::string>(item)), handler);
	}
	else if (item.type() == typeid(std::vector<byte>))
	{
		auto handler = boost::bind(&TCPConnection::HandleWriteAsync, shared_from_this(), boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred);
		boost::asio::async_write(m_socket, boost::asio::buffer(boost::get<std::vector<byte>>(item)), handler);
	}
	else
		ASSERT(false);
}

void TCPConnection::HandleWriteAsync(const boost::system::error_code& error, size_t bytesTransferred)
{
	if (!IsConnected())
		return;

	if (!error)
	{
		const auto& item = m_sendQueue.front();
		const size_t itemRealSize = (item.type() == typeid(std::string)) ? boost::get<std::string>(item).length() : boost::get<std::vector<byte>>(item).size();

		if (itemRealSize == bytesTransferred)
		{
			m_sendQueue.pop_front();
			LOG << bytesTransferred << " bytes has been sent successfully to " << GetEndpoint() << LogSeverity::debug;
		}
		else
		{
			LOG << itemRealSize << " bytes expected to be sent to " << GetEndpoint() << " but " << bytesTransferred << " bytes has been transferred" << LogSeverity::error;
			Close();
		}
	}
	else
		OnReceivedErrorInternal(error);
}

// void TCPConnection::ReadSome(boost::asio::streambuf& streambuf, size_t length, std::function<void(size_t)> externalHandler)
// {
// 	auto handler = boost::bind(&TCPConnection::HandleReadInternal, shared_from_this(), externalHandler, boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred);
// 
// 	boost::asio::async_read(GetSocket(), streambuf, boost::asio::transfer_exactly(length), handler);
// }
