#pragma once
#include "TCPServer.h"
#include "UsersManager.h"

class ApoapseServer
{
	std::unique_ptr<TCPServer> m_mainServer;
	boost::asio::io_service m_mainServerIOService;

public:
	std::unique_ptr<UsersManager> usersManager;

	//ApoapseServer();
// 	virtual ~ApoapseServer();
	void StartMainServer(UInt16 port);

private:
};