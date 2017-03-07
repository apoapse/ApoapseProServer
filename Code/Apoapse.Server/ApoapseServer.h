#pragma once
#include "Database.h"
#include "SQLQuery.h"
class TCPServer;
class UsersManager;
class RemoteServersManager;

class ApoapseServer
{
	boost::asio::io_service m_IOServiceForClients, m_IOServiceForRemoteServers;
	std::unique_ptr<TCPServer> m_ServerForClients;
	std::unique_ptr<TCPServer> m_ServerForRemoteServers;
	std::unique_ptr<RemoteServersManager> m_remoteServersManager;
	std::unique_ptr<UsersManager> m_usersManager;

public:
	ApoapseServer(UInt16 clientsPort, UInt16 serversPort, Database& database);
	~ApoapseServer();

	void Start();
	UsersManager& GetUsersManager() const;
	RemoteServersManager& GetRemoteServersManager() const;
	Database& database;

private:

};