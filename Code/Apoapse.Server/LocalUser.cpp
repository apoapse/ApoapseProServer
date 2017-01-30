#include "stdafx.h"
#include "LocalUser.h"
#include "Common.h"

LocalUser::LocalUser(ApoapseServer& apoapseServer, UsersManager& usersManager, const ApoapseAddress::UsernameHash& usernameHash) : m_usernameHash(usernameHash),
	m_usersManager(usersManager),
	server(apoapseServer)
{
	m_usersManager.AddConnectedUser(this);

	LOG_DEBUG_ONLY("Created new user " << usernameHash.GetStr());
}

LocalUser::~LocalUser()
{
	m_usersManager.RemoveConnectedUser(this);

	LOG << "User " << GetUsernameHash() << " disconnected";
}

std::shared_ptr<LocalUser> LocalUser::GetPtr()
{
	return shared_from_this();
}

void LocalUser::AssociateConnection(ClientConnection* connection)
{
	m_connections.insert(connection);

	LOG << "Added connection " << connection->GetEndpoint() << " to connected user " << GetUsernameHash();
}

void LocalUser::RemoveAssociatedConnection(ClientConnection* connection)
{
	m_connections.erase(connection);
}

ApoapseAddress::UsernameHash LocalUser::GetUsernameHash() const
{
	return m_usernameHash;
}

void LocalUser::Send(const std::vector<byte>& bytes)
{
	for (auto& connection : m_connections)
	{
		connection->Send(bytes);
	}
}

void LocalUser::Send(const std::string& str)
{
	for (auto& connection : m_connections)
	{
		connection->Send(str);
	}
}
