#include "stdafx.h"
#include "LocalUser.h"
#include "Common.h"

LocalUser::LocalUser(UsersManager& usersManager, const ApoapseAddress::UsernameHash& usernameHash)
	: m_usernameHash(std::move(usernameHash)),
	m_usersManager(usersManager)
{
	m_usersManager.AddConnectedUser(this);

	LOG_DEBUG_ONLY("Created new user " << usernameHash.GetStr());
}

LocalUser::~LocalUser()
{
	m_usersManager.RemoveConnectedUser(this);

	LOG << "User " << m_usernameHash.GetStr() << " disconnected";
}

std::shared_ptr<LocalUser> LocalUser::GetPtr()
{
	return shared_from_this();
}

void LocalUser::AssociateConnection(ClientConnection* connection)
{
	m_connections.insert(connection);

	LOG << "Added connection " << connection->GetEndpoint() << " to connected user " << GetUsernameHash().GetStr();
}

void LocalUser::RemoveAssociatedConnection(ClientConnection* connection)
{
	m_connections.erase(connection);
}

ApoapseAddress::UsernameHash LocalUser::GetUsernameHash() const
{
	return m_usernameHash;
}
