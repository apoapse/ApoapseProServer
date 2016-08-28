#pragma once
#include <boost\asio.hpp>
#include "TCPConnection_OLD.h"

class TCPServer
{
	using boostTCP = boost::asio::ip::tcp;

	boost::asio::io_service& m_ioservice;
	boostTCP::acceptor* m_acceptor;

public:
	enum Protocol
	{
		IP_v4,
		IP_v6
	};

	TCPServer(boost::asio::io_service& io_service, const unsigned short port, const Protocol IPProtocol = IP_v4);
	~TCPServer();
	void Close();

private:
	void HandleAcceptAsync(const TCPConnection_OLD::TCPConnection::pointer, const boost::system::error_code& error);
	void StartAccept();
};