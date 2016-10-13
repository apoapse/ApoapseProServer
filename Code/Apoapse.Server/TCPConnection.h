#pragma once
#include "ByteUtils.h"
#include <boost\asio.hpp>
#include <boost\enable_shared_from_this.hpp>
#include <boost\bind.hpp>

#define SOCKET_READ_BUFFER_SIZE 1024
#define HEADER_LENGTH 1024

class TCPConnection : public boost::enable_shared_from_this<TCPConnection>
{
	using boostTCP = boost::asio::ip::tcp;
	friend class TCPServer;

private:
	boostTCP::socket m_socket;
	bool m_isConnected;

protected:
	std::array<byte, SOCKET_READ_BUFFER_SIZE> m_readContentBuffer;
	byte m_readHeaderBuffer[HEADER_LENGTH];
	boost::asio::streambuf data_;
	bool m_isNetMessageCreated;

public:
	typedef boost::shared_ptr<TCPConnection> pointer;

	TCPConnection(boost::asio::io_service& io_service) : m_socket(io_service), m_isConnected(false), m_isNetMessageCreated(false)
	{
	}

	~TCPConnection()
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

	void Connect(const std::string& adress, const unsigned short port)
	{
		ASSERT(!IsConnected());
		auto destination = boostTCP::endpoint(boost::asio::ip::address::from_string(adress), port);

		m_socket.async_connect(destination, boost::bind(&TCPConnection::HandleConnectAsync, shared_from_this(), boost::asio::placeholders::error));
	}

	void Send(const boost::asio::const_buffer& inputBuffer)
	{
		auto handler = boost::bind(&TCPConnection::HandleWriteAsync, shared_from_this(), boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred);
		boost::asio::async_write(m_socket, boost::asio::buffer(inputBuffer), handler);	// #TODO use transport layer
	}

	boost::asio::ip::tcp::endpoint GetEndpoint() const
	{
		return m_socket.remote_endpoint();
	}

	bool IsConnected() const
	{
		return m_isConnected;
	}

	void Close()
	{
		m_isConnected = false;
		m_socket.close();
	}

private:
	void HandleConnectAsync(const boost::system::error_code& error)
	{
		if (!error)
			m_isConnected = true;

		if (this->OnConnectedToServer(error))
		{
			#ifdef DEBUG
			Log(Format("%1% connected to %2%, port %3%", __FUNCTION__, GetEndpoint().address(), GetEndpoint().port()), LogSeverity::debug);
			#endif

			ListenIncomingNewMessages();
		}
		else
			Close();
	}

	void HandleAcceptedAsync(const boost::system::error_code& error)
	{
		#ifdef DEBUG
		Log(Format("%1% accepeted by server %2%, port %3%", __FUNCTION__, GetEndpoint().address(), GetEndpoint().port()), LogSeverity::debug);
		#endif

		HandleConnectAsync(error);
	}

	void ListenIncomingNewMessages()
	{
		ASSERT(IsConnected());

		auto handler = boost::bind(&TCPConnection::ReadReceivedHeader, shared_from_this(), boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred);
		boost::asio::async_read(m_socket, boost::asio::buffer(m_readHeaderBuffer), boost::asio::transfer_at_least(HEADER_LENGTH), handler);
	}

	void ListenIncomingMessageContent()
	{
		auto handler = boost::bind(&TCPConnection::ReadReceivedContent, shared_from_this(), boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred);
		boost::asio::async_read(m_socket, boost::asio::buffer(m_readContentBuffer), handler);
	}

	/*void HandleReadAsync(const boost::system::error_code& error, size_t bytesTransferred)
	{
		if (error)
		{
			if (!this->OnReadError(error))
			{
				Close();
				return;
			}
		}
		else
			ReadReceivedContent(bytesTransferred);

		ListenIncomingMessages();	// Needed for a persistent connection
	}*/

	void ReadReceivedHeader(const boost::system::error_code& error, size_t bytesTransferred)
	{
		ASSERT(bytesTransferred == HEADER_LENGTH);	//TODO

		#ifdef DEBUG
		Log(Format("%1% received %4% bytes from %2%, port %3%", __FUNCTION__, GetEndpoint().address(), GetEndpoint().port(), bytesTransferred), LogSeverity::debug);
		#endif

		if (error)
		{
			if (!this->OnReadError(error))
			{
				Close();
				return;
			}
		}

		if (m_isNetMessageCreated)
			ASSERT(false);	//TODO

		//	Read header
		UInt32 expectedContentSize = ByteConverter::ToUInt32(m_readHeaderBuffer);

//		m_tempNetMessage = new NetMessage();
	}

	void ReadReceivedContent(const boost::system::error_code& error, size_t bytesTransferred)
	{
		#ifdef DEBUG
		Log(Format("%1% received %4% bytes from %2%, port %3% (%5% bytes left)", __FUNCTION__, GetEndpoint().address(), GetEndpoint().port(), bytesTransferred, -1), LogSeverity::debug);//TODO
		#endif
	}

	void HandleWriteAsync(const boost::system::error_code& error, size_t bytesTransferred)
	{
		#ifdef DEBUG
		Log(Format("%1% send %4% bytes to %2%, port %3%", __FUNCTION__, GetEndpoint().address(), GetEndpoint().port(), bytesTransferred), LogSeverity::debug);
		#endif

		if (!this->OnSentPacket(error, bytesTransferred))
			Close();
	}

protected:
	virtual bool OnConnectedToServer(const boost::system::error_code& error) = 0;
	virtual bool OnReceivedPacket(/*const NetMessage* netMessage*/) = 0;
	virtual bool OnReadError(const boost::system::error_code& error) = 0;
	virtual bool OnSentPacket(const boost::system::error_code& error, size_t bytesTransferred) = 0;
};