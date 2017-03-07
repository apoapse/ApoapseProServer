#include "stdafx.h"
#include "LocalUser.h"
#include "Common.h"

LocalUser::LocalUser(UsersManager& usersManager, const ApoapseAddress::UsernameHash& usernameHash, DbId id) : m_usernameHash(usernameHash),
	m_usersManager(usersManager),
	m_id(id)
{
	m_usersManager.AddConnectedUser(this);

	LOG << "User " << usernameHash.GetStr() << " (id:" << m_id << ") connected via a new user object";
}

LocalUser::~LocalUser()
{
	m_usersManager.RemoveConnectedUser(this);

	LOG << "User " << GetUsernameHash() << " disconnected";
}

std::shared_ptr<LocalUser> LocalUser::GetShared()
{
	return shared_from_this();
}

void LocalUser::AssociateConnection(ClientConnection* connection)
{
	m_connections.insert(connection);

	LOG << "Associated connection " << connection->GetEndpoint() << " to user " << GetUsernameHash();
}

void LocalUser::RemoveAssociatedConnection(ClientConnection* connection)
{
	m_connections.erase(connection);
}

ApoapseAddress::UsernameHash LocalUser::GetUsernameHash() const
{
	return m_usernameHash;
}

ApoapseAddress LocalUser::GetFullAddress() const
{
	return ApoapseAddress(UsersManager::GetCurrentServerDomain(), GetUsernameHash());
}

void LocalUser::Send(std::shared_ptr<std::vector<byte>> bytesPtr, TCPConnection* excludedConnection/* = nullptr*/)
{
	for (auto& connection : m_connections)
	{
		if (excludedConnection != nullptr && connection->GetEndpoint() == excludedConnection->GetEndpoint())
			continue;

		connection->Send(std::move(bytesPtr));
	}
}

void LocalUser::Send(std::unique_ptr<std::string> strPtr, TCPConnection* excludedConnection/* = nullptr*/)
{
	for (auto& connection : m_connections)
	{
		if (excludedConnection != nullptr && connection->GetEndpoint() == excludedConnection->GetEndpoint())
			continue;

		connection->Send(std::move(strPtr));
	}
}

DbId LocalUser::GetDatabaseId() const
{
	return m_id;
}

size_t LocalUser::GetConnectionsCount() const
{
	return m_connections.size();
}

void LocalUser::Disconnect()
{
	for (auto& connection : m_connections)
		connection->Close();
}
