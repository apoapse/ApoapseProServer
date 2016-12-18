#pragma once
#include "ByteUtils.h"
#include <boost\asio.hpp>
#include <boost\bind.hpp>	//TODO: remove
#include <functional>

#define SOCKET_READ_BUFFER_SIZE 1024
//#define HEADER_LENGTH 4

class TCPConnection : public std::enable_shared_from_this<TCPConnection>
{
	using boostTCP = boost::asio::ip::tcp;
	friend class TCPServer;

private:
	boostTCP::socket m_socket;
	bool m_isConnected = false;
	//std::deque<std::shared_ptr<NetMessage>> m_sendQueue;
	boost::asio::io_service::strand m_writeStrand;

protected:
 	//std::array<byte, SOCKET_READ_BUFFER_SIZE> m_readBuffer;
	//boost::asio::streambuf m_readStreamBuffer;

public:
	typedef std::shared_ptr<TCPConnection> TCPConnection_ptr;

	TCPConnection(boost::asio::io_service& io_service);

	virtual ~TCPConnection()
	{
	}

	template<typename T_CONNECTION>
	static TCPConnection_ptr Create(boost::asio::io_service& io_service)
	{
		return std::make_shared<T_CONNECTION>(io_service);
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

	void ListenForCommandName();
	void ListenForInlineCommandContent();

	/*void ReadHeader(const boost::system::error_code& error, size_t bytesTransferred);*/
	

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


	virtual bool OnConnectedToServer() = 0;
	//virtual void OnReceivedCommandName(const boost::system::error_code& error, size_t bytesTransferred) = 0;
	//virtual bool OnReceivedPacket(std::shared_ptr<NetMessage> netMessage) = 0;
	//virtual bool OnReceivedContent(/*const NetMessage& netMessage*/) = 0;	// Called each time a part of NetMessage is received which can be partial data
	virtual bool OnReceivedError(const boost::system::error_code& error) = 0;
	//virtual bool OnSentPacket(const std::shared_ptr<NetMessage> netMessage, size_t bytesTransferred) = 0;
};