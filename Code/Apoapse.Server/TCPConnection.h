#pragma once
#include "ByteUtils.h"
#include <boost\asio.hpp>
#include <boost\bind.hpp>
#include <functional>
#include <atomic>

class TCPConnection : public std::enable_shared_from_this<TCPConnection>
{
	using boostTCP = boost::asio::ip::tcp;
	friend class TCPServer;

private:
	boostTCP::socket m_socket;
	std::atomic<bool> m_isConnected = { false };
	boost::asio::io_service::strand m_writeStrand;


public:
	typedef std::shared_ptr<TCPConnection> TCPConnection_ptr;

	TCPConnection(boost::asio::io_service& io_service);

	virtual ~TCPConnection()
	{
	}

	boostTCP::socket& GetSocket()
	{
		return m_socket;
	}

	boost::asio::ip::tcp::endpoint GetEndpoint() const;
	void Connect(const std::string& adress, const UInt16 port);
	bool IsConnected() const;
	void Close();

private:
	void HandleConnectAsync(const boost::system::error_code& error);
	void HandleAcceptedAsync(const boost::system::error_code& error);
	void OnReceivedErrorInternal(const boost::system::error_code& error);

	void HandleReadInternal(const std::function<void(size_t)>& handler, const boost::system::error_code& error, size_t bytesTransferred);

	void QueueSend(const std::vector<byte> data);
	void InternalSend();
	void HandleWriteAsync(const boost::system::error_code& error, size_t bytesTransferred);

protected:
	template <typename T>
	void ReadUntil(boost::asio::streambuf& streambuf, T delimiter, std::function<void(size_t)> externalHandler)
	{
		auto handler = boost::bind(&TCPConnection::HandleReadInternal, shared_from_this(), externalHandler, boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred);

		boost::asio::async_read_until(GetSocket(), streambuf, delimiter, handler);
	}

	//void ReadSome(boost::asio::streambuf& streambuf, size_t length, std::function<void(size_t)> externalHandler);

	virtual bool OnConnectedToServer() = 0;
	virtual bool OnReceivedError(const boost::system::error_code& error) = 0;
};