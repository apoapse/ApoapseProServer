#include "stdafx.h"
#include "User.h"
#include "Common.h"
#include "ServerConnection.h"
#include "ApoapseServer.h"
#include "ByteUtils.hpp"

User::Username::Username(const std::vector<byte>& hash) : m_usernameHash()
{
	SECURITY_ASSERT(hash.size() == sha256Length);

	std::copy(hash.begin(), hash.end(), m_usernameHash.begin());
}

User::User(DbId databaseId, const Username& username, ServerConnection* connection, ApoapseServer* apoapseServer)
	: m_username(username)
	, server(apoapseServer)
	, m_databaseId(databaseId)
{
	ASSERT_MSG(!server->usersManager->IsUserConnected(username), "Trying to create a new user object ");
		
	AddConnection(connection);

	LOG << "User " << GetUsername() << " authenticated";
}

User::~User()
{
	LOG << "User " << GetUsername() << " disconnected";
	server->usersManager->RemoveConnectedUser(*this);
}

const User::Username& User::GetUsername() const
{
	return m_username;
}

std::shared_ptr<User> User::GetObjectShared()
{
	return shared_from_this();
}

void User::RemoveConnection(ServerConnection* connection)
{
	m_associatedConnections.erase(connection);
}

void User::AddConnection(ServerConnection* connection)
{
	m_associatedConnections.insert(connection);

	LOG << "Added connection " << connection->GetEndpoint() << " to user " << GetUsername();
}

void User::Send(BytesWrapper bytesPtr, TCPConnection* excludedConnection /*= nullptr*/)
{
	for (auto* connection : m_associatedConnections)
	{
		connection->Send(bytesPtr, excludedConnection);
	}
}

void User::Send(StrWrapper strPtr, TCPConnection* excludedConnection /*= nullptr*/)
{
	for (auto* connection : m_associatedConnections)
	{
		connection->Send(std::move(strPtr), excludedConnection);
	}
}

void User::Send(std::unique_ptr<class NetworkPayload> data, TCPConnection* excludedConnection /*= nullptr*/)
{
	for (auto* connection : m_associatedConnections)
	{
		connection->Send(std::move(data), excludedConnection);
	}
}

std::string User::GetEndpointStr() const
{
	return "user " + GetUsername().ToStr() +" (" + std::to_string(m_associatedConnections.size()) + " connections)";
}

void User::Close()
{
	for (auto* connection : m_associatedConnections)
		connection->Close();
}
