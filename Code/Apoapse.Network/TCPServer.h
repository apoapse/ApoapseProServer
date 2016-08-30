#pragma once
#include <boost\asio.hpp>
#include <memory>
#include "TCPConnection.h"
#include "Diagnostics.h"

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

	TCPServer(boost::asio::io_service& io_service, const unsigned short port, const Protocol IPProtocol = IP_v4) : m_acceptor(std::make_unique<boostTCP::acceptor>(io_service)), m_ioservice(io_service)
	{
		boost::system::error_code error;
		boostTCP::endpoint endpoint;

		if (IPProtocol == Protocol::IP_v4)
			endpoint = boostTCP::endpoint(boostTCP::v4(), port);

		else if (IPProtocol == Protocol::IP_v6)
			endpoint = boostTCP::endpoint(boostTCP::v6(), port);

		m_acceptor->open(endpoint.protocol(), error);
		m_acceptor->set_option(boostTCP::acceptor::reuse_address(true));

		m_acceptor->bind(endpoint, error);

		if (!error)
		{
			#ifdef DEBUG
			Log(Format("%1% server started at %2%, port %3%, protocol %4%", __FUNCTION__, endpoint.address(), endpoint.port(), endpoint.protocol().protocol()), LogSeverity::debug);
			#endif

			m_acceptor->listen();
		}
		else
			throw error;
	}

	~TCPServer()
	{
	}

	void Close()
	{
		m_acceptor->close();
	}

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