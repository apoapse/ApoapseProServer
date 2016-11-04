#pragma once
#include "ByteUtils.h"
#include <boost\asio.hpp>
#include <boost\bind.hpp>
#include "NetMessage.h"

#define SOCKET_READ_BUFFER_SIZE 1024
#define HEADER_LENGTH 4

class TCPConnection : public boost::enable_shared_from_this<TCPConnection>
{
	using boostTCP = boost::asio::ip::tcp;
	friend class TCPServer;

private:
	boostTCP::socket m_socket;
	bool m_isConnected = false;
	std::shared_ptr<NetMessage> m_currentNetMessage;
	std::deque<std::shared_ptr<NetMessage>> m_sendQueue;
	boost::asio::io_service::strand m_writeStrand;

protected:
	std::array<byte, SOCKET_READ_BUFFER_SIZE> m_readContentBuffer;
	byte m_readHeaderBuffer[HEADER_LENGTH];
	boost::asio::streambuf data_;

public:
	typedef boost::shared_ptr<TCPConnection> pointer;
	static const UInt16 headerLength = HEADER_LENGTH;

	TCPConnection(boost::asio::io_service& io_service);

	virtual ~TCPConnection()
	{
	}

	template<typename T_CONNECTION>
	static pointer Create(boost::asio::io_service& io_service)
	{
		return pointer(new T_CONNECTION(io_service));
	}

	boostTCP::socket& GetSocket()
	{
		return m_socket;
	}

	boost::asio::ip::tcp::endpoint GetEndpoint() const;
	void Connect(const std::string& adress, const UInt16 port);
	bool IsConnected() const;
	void Close();
	void Send(const std::string& msg);

private:
	void HandleConnectAsync(const boost::system::error_code& error);
	void HandleAcceptedAsync(const boost::system::error_code& error);

	void ListenIncomingNewMessages();
	void ListenIncomingMessageContent();

	void ReadHeader(const boost::system::error_code& error, size_t bytesTransferred);
	void ReadReceivedContent(const boost::system::error_code& error, size_t bytesTransferred);

	void QueueSendNetMessage(const std::shared_ptr<NetMessage> netMessage);
	void InternalSend();
	void HandleWriteAsync(const boost::system::error_code& error, size_t bytesTransferred);

protected:
	virtual bool OnConnectedToServer(const boost::system::error_code& error) = 0;
	virtual bool OnReceivedPacket(std::shared_ptr<NetMessage> netMessage) = 0;
	//virtual bool OnReceivedContent(/*const NetMessage& netMessage*/) = 0;	// Called each time a part of NetMessage is received which can be partial data
	virtual bool OnReadError(const boost::system::error_code& error) = 0;
	virtual bool OnSentPacket(const std::shared_ptr<NetMessage> netMessage, size_t bytesTransferred) = 0;
};