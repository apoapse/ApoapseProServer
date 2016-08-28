#include "stdafx.h"
#include "TCPConnection_OLD.h"
#include <boost\bind.hpp>
#include "Common.h"

using namespace TCPConnection_OLD;

TCPConnection::TCPConnection(boost::asio::io_service& io_service) : m_socket(io_service), m_isConnected(false)
{
}

TCPConnection::~TCPConnection()
{
}

void TCPConnection::Connect(const std::string& adress, const unsigned short port)
{
	Log("TCPConnection Trying to connect", LogSeverity::verbose);
	auto destination = boostTCP::endpoint(boost::asio::ip::address::from_string(adress), port);

	m_socket.async_connect(destination, boost::bind(&TCPConnection::HandleConnectAsync, shared_from_this(), boost::asio::placeholders::error));
}

void TCPConnection::HandleConnectAsync(const boost::system::error_code& error)
{
	if (!error)
	{
		m_isConnected = true;
		Log("TCPConnection connected to port " + std::to_string(GetEndpoint().port()));
		Send(boost::asio::buffer("hi"));	//TEST

		ListenIncomingData();
	}
	else
	{
		auto test3 = (boost::asio::error::basic_errors)error.value();

		Log("TCPConnection Error durring connection attempt", LogSeverity::error);
		Close();	// ?
	}
}

void TCPConnection::OnAccepted()
{
	m_isConnected = true;
	Log("TCPConnection accepted");

	// Start lisening to incoming data
	ListenIncomingData();

	Send(boost::asio::buffer("HELLO TEST"));	//TEMP
}

void TCPConnection::Send(const boost::asio::const_buffer& inputBuffer)
{
	auto handler = boost::bind(&TCPConnection::HandleWriteAsync, shared_from_this(), boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred);
	boost::asio::async_write(m_socket, boost::asio::buffer(inputBuffer), handler);
}

void TCPConnection::HandleWriteAsync(const boost::system::error_code& error, size_t bytes_transferred)
{
	if (error)
	{
		Log("handle_write Error, closing socket", LogSeverity::error);
		Close();

		//TODO: Make sure the object is deleted
	}
}

void TCPConnection::ListenIncomingData()
{
	auto handler = boost::bind(&TCPConnection::HandleReadAsync, shared_from_this(), boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred);
	m_socket.async_read_some(boost::asio::buffer(m_readBuffer), handler);

	//boost::asio::async_read_until(m_socket, m_inputBuffer, '\n', handler);	//TODO: use this system instead or read some? WARNING: The buffer might have some excess data that would need to be parsed 
}

void TCPConnection::HandleReadAsync(const boost::system::error_code& error, size_t bytes_transferred)
{
	if (!error)
	{
		std::string output(m_readBuffer, m_readBuffer + sizeof(m_readBuffer));

		//	Remove empty chars and C null char from the buffer
		auto pos = output.find_first_of('\r');	//TEST ONLY
		output.resize(pos);

		Log(output);

		ListenIncomingData();
	}
	else
	{
		Log("handle_read Error, closing socket", LogSeverity::error);
		Close();

		//TODO: Make sure the object is deleted
	}
}

boost::asio::ip::tcp::endpoint TCPConnection::GetEndpoint() const
{
	return m_socket.remote_endpoint();
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