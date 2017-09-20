#include "stdafx.h"
#include "User.h"
#include "Common.h"
#include "ServerConnection.h"
#include "ApoapseServer.h"
#include "ByteUtils.hpp"

bool User::Address::operator<(const Address& other) const
{
	return (username < other.username);
}

std::string User::Address::ToStr() const
{
	return BytesToHexString(username);
}

bool User::Address::operator==(const Address& other) const
{
	return (username == other.username);
}

User::User(const Address& address, ServerConnection* connection, ApoapseServer* apoapseServer)
	: m_address(address)
	, server(apoapseServer)
{
	ASSERT_MSG(!server->usersManager->IsUserConnected(address), "Trying to create a new user object ");
		
	AddConnection(connection);

	LOG << "User " << GetAddress() << " authenticated";
}

User::~User()
{
	LOG << "User " << GetAddress() << " disconnected";
	server->usersManager->RemoveConnectedUser(*this);
}

User::Address User::GetAddress() const
{
	return m_address;
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

	LOG << "Added connection " << connection->GetEndpoint() << " to user " << GetAddress();
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
	return "user " + GetAddress().ToStr() +" (" + std::to_string(m_associatedConnections.size()) + " connections)";
}

void User::Close()
{
	for (auto* connection : m_associatedConnections)
		connection->Close();
}
