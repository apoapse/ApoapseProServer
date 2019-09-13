#pragma once
#include "TCPServer.h"
#include "TCPServerNoTLS.h"
class UsersManager;
class UsergroupManager;

class ApoapseServer
{
	std::unique_ptr<TCPServer> m_mainServer;
	std::unique_ptr<TCPServerNoTLS> m_filesServer;
	ssl::context m_tlsContext;

public:
	UsersManager* usersManager = nullptr;
	UsergroupManager* usergroupManager = nullptr;

	boost::asio::io_service fileServerIOService;
	boost::asio::io_service mainServerIOService;

	ApoapseServer();
 	virtual ~ApoapseServer();
	void SetupMainServer(UInt16 port);
	void SetupFilesServer(UInt16 port);
	void StartIOServices();

private:
};