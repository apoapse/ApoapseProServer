#pragma once
#include "Diagnostics.h"
#include <boost\asio.hpp>
#include <boost\enable_shared_from_this.hpp>
#include <boost\bind.hpp>
#include "Common.h"

#define SOCKET_READ_BUFFER_SIZE 1024	// #TODO: find the most relevant value

class TCPConnection : public boost::enable_shared_from_this<TCPConnection>
{
	using boostTCP = boost::asio::ip::tcp;
	friend class TCPServer;

private:
	boostTCP::socket m_socket;
	bool m_isConnected;

protected:
	char m_readBuffer[SOCKET_READ_BUFFER_SIZE];

public:
	typedef boost::shared_ptr<TCPConnection> pointer;

	TCPConnection(boost::asio::io_service& io_service) : m_socket(io_service), m_isConnected(false)
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
		auto handler = boost::bind(&TCPConnection::HandleWriteAsync, shared_from_this(), boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred);	// #TODO: declare the handler in a member var instead of creating it at each call?
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

			ListenIncomingData();
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

	void ListenIncomingData()
	{
		auto handler = boost::bind(&TCPConnection::HandleReadAsync, shared_from_this(), boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred);	// #TODO: declare the handler in a member var instead of creating it at each call?
		m_socket.async_receive(boost::asio::buffer(m_readBuffer), handler);
	}

	void HandleReadAsync(const boost::system::error_code& error, size_t bytesTransferred)
	{
		#ifdef DEBUG
		Log(Format("%1% received %4% bytes from %2%, port %3%", __FUNCTION__, GetEndpoint().address(), GetEndpoint().port(), bytesTransferred), LogSeverity::debug);
		#endif

		if (this->OnReceivedPacket(error, bytesTransferred))
			ListenIncomingData();	// Needed for a persistent connection
		else
			Close();
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
	virtual bool OnReceivedPacket(const boost::system::error_code& error, size_t bytesTransferred) = 0;
	virtual bool OnSentPacket(const boost::system::error_code& error, size_t bytesTransferred) = 0;
};