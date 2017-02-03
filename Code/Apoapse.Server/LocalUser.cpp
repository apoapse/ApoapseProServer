#include "stdafx.h"
#include "LocalUser.h"
#include "Common.h"

LocalUser::LocalUser(UsersManager& usersManager, const ApoapseAddress::UsernameHash& usernameHash, Int64 id)
	: m_usernameHash(usernameHash),
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
