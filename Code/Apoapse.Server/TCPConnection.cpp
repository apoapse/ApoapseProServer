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
	LOG_DEBUG << "TCPConnection accepted from " << GetEndpoint();
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

void TCPConnection::Send(std::shared_ptr<std::vector<byte>> bytesPtr, TCPConnection* excludedConnection/* = nullptr*/)
{
	ASSERT(excludedConnection == nullptr);

	const bool isWriteInProgress = !m_sendQueue.empty();
	m_sendQueue.push_back(bytesPtr);

	if (!isWriteInProgress)
		InternalSend();

	// #TODO #IMPORTANT Use the following code to make the Send operation thread safe
	// m_socket.get_io_service().post(m_writeStrand.wrap([bytesPtr = std::move(bytesPtr), this]() mutable
	// 	{
	// 
	// 	}));
}

void TCPConnection::Send(std::unique_ptr<std::string> strPtr, TCPConnection* excludedConnection/* = nullptr*/)
{
	ASSERT(excludedConnection == nullptr);

	const bool isWriteInProgress = !m_sendQueue.empty();
	m_sendQueue.push_back(std::move(strPtr));

	if (!isWriteInProgress)
		InternalSend();

	// #TODO #IMPORTANT Use the following code to make the Send operation thread safe
// 	m_socket.get_io_service().post(m_writeStrand.wrap([strPtr = std::move(strPtr), this]() mutable
// 	{
// 
// 	}));
}

void TCPConnection::InternalSend()
{
	const auto& item = m_sendQueue.front();

	if (item.type() == typeid(std::unique_ptr<std::string>))
	{
		auto handler = boost::bind(&TCPConnection::HandleWriteAsync, shared_from_this(), boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred);
		boost::asio::async_write(m_socket, boost::asio::buffer(*boost::get<std::unique_ptr<std::string>>(item)), handler);
	}
	else if (item.type() == typeid(std::shared_ptr<std::vector<byte>>))
	{
		auto handler = boost::bind(&TCPConnection::HandleWriteAsync, shared_from_this(), boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred);
		boost::asio::async_write(m_socket, boost::asio::buffer(*boost::get<std::shared_ptr<std::vector<byte>>>(item)), handler);
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
		const size_t itemRealSize = (item.type() == typeid(std::unique_ptr<std::string>)) ? boost::get<std::unique_ptr<std::string>>(item)->length() : boost::get<std::shared_ptr<std::vector<byte>>>(item)->size();

		if (itemRealSize == bytesTransferred)
		{
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

	m_sendQueue.pop_front();

	if (!m_sendQueue.empty())
		InternalSend();
}