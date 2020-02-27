// ----------------------------------------------------------------------------
// Copyright (C) 2020 Apoapse
// Copyright (C) 2020 Guillaume Puyal
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at http://www.apache.org/licenses/LICENSE-2.0
//
// For more information visit https://github.com/apoapse/
// And https://apoapse.space/
// ----------------------------------------------------------------------------

#include "stdafx.h"
#include "User.h"
#include "Common.h"
#include "ServerConnection.h"
#include "ApoapseServer.h"
#include "UsersManager.h"
#include "Random.hpp"
#include "Hash.hpp"
#include "MemoryUtils.hpp"
#include "SQLQuery.h"
#include "UsergroupManager.h"
#include "DataStructure.h"
#include "ServerFileStreamConnection.h"

User::User(DataStructure& data, ServerConnection* connection, ApoapseServer* apoapseServer) : server(apoapseServer)
{
	m_username = data.GetField("username").GetValue<Username>();
	ASSERT_MSG(!server->usersManager->IsUserConnected(m_username), "Trying to create a new user object but the user already exist");

	m_databaseId = data.GetDbId();
	m_usergroup = &server->usergroupManager->GetUsergroup(data.GetField("usergroup").GetValue<Uuid>());
	m_isUsingTemporaryPassword = data.GetField("is_temporary_password").GetValue<bool>();

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

const Usergroup& User::GetUsergroup() const
{
	return *m_usergroup;
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

void User::RequestClose()
{
	for (auto* connection : m_associatedConnections)
		connection->RequestClose();
}

bool User::IsUsingTemporaryPassword() const
{
	return m_isUsingTemporaryPassword;
}

FileStreamAuth User::GenerateFileStreamAuthCode(ServerConnection& caller)
{
	FileStreamAuth auth;
	auth.code = VectorToArray<byte, sha256Length>(Cryptography::GenerateRandomBytes(sha256Length));
	auth.caller = &caller;

	m_fileStreamAuthCode = auth;
	
	return m_fileStreamAuthCode.value();
}

bool User::AuthenticateFileStream(const hash_SHA256& authCode, ServerFileStreamConnection* fileStream)
{
	if (!m_fileStreamAuthCode.has_value())
	{
		return false;
	}
	
	if (m_fileStreamAuthCode.value().code == authCode && m_fileStreamAuthCode.value().caller != nullptr)
	{
		LOG << "File stream connection authenticated for user " << GetUsername().ToStr();

		m_fileStreamAuthCode.value().caller->SetRelatedFileStream(fileStream);
		fileStream->SetMainConnection(m_fileStreamAuthCode.value().caller);
		fileStream->SetAuthenticated();
		m_fileStreamAuthCode.reset();
		
		return true;
	}
	else
	{
		return false;
	}
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

std::string User::GetNickname() const
{
	SQLQuery query(*global->database);
	query << SELECT << "nickname" << FROM << "users" << WHERE << "username" << EQUALS << m_username.GetRaw();
	auto res = query.Exec();

	return res[0][0].GetText();
}

std::string User::GetAvatarFilePath(const Username& username)
{
	return "server_userfiles/av_" + username.ToStr().substr(0, 24) + ".dat";
}
