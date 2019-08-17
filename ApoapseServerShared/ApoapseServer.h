#pragma once
#include "TCPServer.h"
class UsersManager;
class UsergroupManager;

class ApoapseServer
{
	std::unique_ptr<TCPServer> m_mainServer;
	std::unique_ptr<TCPServer> m_filesServer;
	boost::asio::io_service m_mainServerIOService;
	boost::asio::io_service m_fileServerIOService;
	ssl::context m_tlsContext;

public:
	UsersManager* usersManager;
	UsergroupManager* usergroupManager;

	ApoapseServer();
 	virtual ~ApoapseServer();
	void SetupMainServer(UInt16 port);
	void SetupFilesServer(UInt16 port);
	void StartIOServices();

private:
};