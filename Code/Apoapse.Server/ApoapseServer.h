#pragma once
#include "TCPServer.h"
#include "UsersManager.h"

class ApoapseServer
{
	boost::asio::io_service m_IOServiceForClients, m_IOServiceForRemoteServers;
	std::unique_ptr<TCPServer> m_ServerForClients;
	std::unique_ptr<TCPServer> m_ServerForRemoteServers;
	std::unique_ptr<UsersManager> m_usersManager;

public:
	ApoapseServer(UInt16 port);
	~ApoapseServer();

	void Start();
	UsersManager& GetUsersManager() const;
	
private:

};