#pragma once
#include "ApoapseAddress.h"
#include <map>
class RemoteServer;
class ApoapseServer;

class RemoteServersManager final
{
	std::map<ApoapseAddress::ServerDomain, RemoteServer*> m_connectedServers;
	boost::asio::io_service& m_ioService;

public:
	RemoteServersManager(boost::asio::io_service& ioService);
	~RemoteServersManager();

	RemoteServer* TryGetRemoteServer(const ApoapseAddress::ServerDomain& domain) const;
	bool IsConnected(const ApoapseAddress::ServerDomain& domain) const;
	
	void AddConnectedServer(RemoteServer* server);
	void RemoveConnectedServer(RemoteServer* server);

	std::shared_ptr<RemoteServer> CreateConnection(ApoapseServer& apoapseServer);
private:
};