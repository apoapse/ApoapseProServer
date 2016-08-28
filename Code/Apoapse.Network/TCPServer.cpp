#include "stdafx.h"
#include "TCPServer.h"
#include <boost\bind.hpp>
#include "Common.h"

using namespace TCPConnection_OLD;

TCPServer::TCPServer(boost::asio::io_service& io_service, unsigned short port, const Protocol IPProtocol) : m_acceptor(new boostTCP::acceptor(io_service)), m_ioservice(io_service)
{
	boostTCP::endpoint endpoint;
	if (IPProtocol == Protocol::IP_v4)
		endpoint = boostTCP::endpoint(boostTCP::v4(), port);

	else if (IPProtocol == Protocol::IP_v6)
		endpoint = boostTCP::endpoint(boostTCP::v6(), port);
	
	try
	{
		m_acceptor->open(endpoint.protocol());

		m_acceptor->set_option(boost::asio::ip::tcp::acceptor::reuse_address(true));

		m_acceptor->bind(endpoint);
		m_acceptor->listen();

		StartAccept();
	}
	catch (const boost::system::error_code& err)
	{
		Log("Start TCP server error " + err.message(), LogSeverity::error);	// #TODO
	}
}

TCPServer::~TCPServer()
{
	Close();
}

void TCPServer::StartAccept()
{
	TCPConnection::pointer newConnection = TCPConnection::Create(m_ioservice);

	auto handler = boost::bind(&TCPServer::HandleAcceptAsync, this, newConnection, boost::asio::placeholders::error);
	m_acceptor->async_accept(newConnection->GetSocket(), handler);

	Log("TCPServer started, ready to accept connections");
}

void TCPServer::HandleAcceptAsync(const TCPConnection::pointer tcpConnection, const boost::system::error_code& error)
{
	if (!error)
	{
		Log("Connection incoming from " + tcpConnection->GetEndpoint().address().to_string() + " " + std::to_string(tcpConnection->GetEndpoint().port()), LogSeverity::verbose);

		tcpConnection->OnAccepted();
	}
	else
	{
		Log("HandleAcceptAsync error", LogSeverity::error);	// #TODO
	}

	// Call StartAccept() again to initiate the next accept operation
	StartAccept();
}

void TCPServer::Close()
{
	m_acceptor->close();
}