#include "stdafx.h"
#include "User.h"
#include "Common.h"
#include "ServerConnection.h"
#include "ApoapseServer.h"
#include "ByteUtils.hpp"
#include "UsersManager.h"
#include "Random.hpp"
#include "Hash.hpp"
#include "MemoryUtils.hpp"
#include "SQLQuery.h"

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

const Username& User::GetUsername() const
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

void User::Send(std::shared_ptr<class NetworkPayload> data, TCPConnection* excludedConnection /*= nullptr*/)
{
	for (auto* connection : m_associatedConnections)
	{
		connection->Send(data, excludedConnection);
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

bool User::IsUsingTemporaryPassword() const	// #MVP
{
	SQLQuery query(*global->database);
	query << SELECT << "is_temporary_passsword" << FROM << "users" << WHERE << "username_hash" << EQUALS << m_username.GetRaw();
	auto res = query.Exec();

	const int useTemporaryPassword = res[0][0].GetInt32();

	return static_cast<bool>(useTemporaryPassword);
}

std::vector<byte> User::GenerateRandomSalt()
{
	return Cryptography::GenerateRandomBytes(sha512Length);
}

std::vector<byte> User::HashPassword(const std::vector<byte>& encryptedPassword, const std::vector<byte>& salt)
{
	const auto res = Cryptography::PBKDF2_SHA512(encryptedPassword, salt, passwordAlgorithmIterations);
	return std::vector<byte>(res.begin(), res.end());
}

bool User::ComparePasswords(const std::vector<byte>& password, const std::vector<byte>& storedPassword, const std::vector<byte>& salt)
{
	SECURITY_ASSERT(storedPassword.size() == salt.size());
	const auto res = Cryptography::PBKDF2_SHA512(password, salt, passwordAlgorithmIterations);

	return (std::vector<byte>(res.begin(), res.end()) == storedPassword);
}

bool User::IsEncryptedPasswordValid(const std::vector<byte>& encryptedPassword)
{
	return (encryptedPassword.size() == sha256Length);
}
