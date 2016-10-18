#pragma once
#include <boost\asio.hpp>
#include <memory>
#include "TCPConnection.h"

class TCPServer
{
	using boostTCP = boost::asio::ip::tcp;

	boost::asio::io_service& m_ioservice;
	std::unique_ptr<boostTCP::acceptor> m_acceptor;

public:
	enum Protocol
	{
		IP_v4,
		IP_v6
	};

	TCPServer(boost::asio::io_service& io_service, const UInt16 port, const Protocol IPProtocol = IP_v4);
	virtual ~TCPServer();

	void Close();

	template <typename T_CONNECTION>
	void StartAccept()
	{
		TCPConnection::pointer newConnection = TCPConnection::Create<T_CONNECTION>(m_ioservice);

		auto handler = boost::bind(&TCPServer::HandleAcceptAsync<T_CONNECTION>, this, newConnection, boost::asio::placeholders::error);	// #TODO: declare the handler in a member var instead of creating it at each call?
		m_acceptor->async_accept(newConnection->GetSocket(), handler);
	}

private:
	template <typename T_CONNECTION>
	void HandleAcceptAsync(const TCPConnection::pointer tcpConnection, const boost::system::error_code& error)
	{
		tcpConnection->HandleAcceptedAsync(error);

		StartAccept<T_CONNECTION>();	// Once one a connection is accepted, we call back the acceptor to handle the next connection
	}
};