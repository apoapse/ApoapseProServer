#pragma once
#include "TCPServer.h"
#include "ReadWriteMutexWrapper.h"
class ApoapseServer
{
	boost::asio::io_service m_IOServiceForClients, m_IOServiceForRemoteServers;
	std::unique_ptr<TCPServer> m_ServerForClients;
	std::unique_ptr<TCPServer> m_ServerForRemoteServers;

public:
	ApoapseServer(UInt16 port);
	~ApoapseServer();

	void Start();
private:

};