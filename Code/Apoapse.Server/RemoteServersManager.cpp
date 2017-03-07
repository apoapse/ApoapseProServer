#include "stdafx.h"
#include "RemoteServersManager.h"
#include "Common.h"
#include "RemoteServer.h"
#include "ApoapseServer.h"

RemoteServersManager::RemoteServersManager(boost::asio::io_service& ioService) : m_ioService(ioService)
{
}

RemoteServersManager::~RemoteServersManager()
{

}

RemoteServer* RemoteServersManager::TryGetRemoteServer(const ApoapseAddress::ServerDomain& domain) const
{
	if (!IsConnected(domain))
		return nullptr;
	else
		return m_connectedServers.at(domain);
}

bool RemoteServersManager::IsConnected(const ApoapseAddress::ServerDomain& domain) const
{
	return (m_connectedServers.count(domain) > 0);
}

void RemoteServersManager::AddConnectedServer(RemoteServer* server)
{
	if (server == nullptr)
		return;

	m_connectedServers[server->GetServerDomain()];
}

void RemoteServersManager::RemoveConnectedServer(RemoteServer* server)
{
	if (server == nullptr)
		return;

	ASSERT(IsConnected(server->GetServerDomain()));

	m_connectedServers.erase(server->GetServerDomain());
}

std::shared_ptr<RemoteServer> RemoteServersManager::CreateConnection(ApoapseServer& apoapseServer)
{
	return std::make_shared<RemoteServer>(m_ioService, RemoteServer::Direction::CURRENT_SERVER_CONNECTED_TO_REMOTE, apoapseServer);
}
