#pragma once
#include "TCPServer.h"
class UsersManager;

class ApoapseServer
{
	std::unique_ptr<TCPServer> m_mainServer;
	boost::asio::io_service m_mainServerIOService;

public:
	UsersManager* usersManager;

	//ApoapseServer();
 	virtual ~ApoapseServer();
	void StartMainServer(UInt16 port);

private:
};