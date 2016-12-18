#pragma once
#include "TCPServer.h"

class ApoapseServer
{
	boost::asio::io_service m_IOServiceGeneral;
	std::unique_ptr<TCPServer> m_tcpServer;

public:
	ApoapseServer(UInt16 port);
	~ApoapseServer();

	void Start();
private:

};