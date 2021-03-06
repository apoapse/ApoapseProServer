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

#pragma once
#include "INetworkSender.h"
#include <set>
#include "Username.h"
#include "IUser.h"
class ServerFileStreamConnection;
class ServerConnection;
class ApoapseServer;
class Usergroup;
class DataStructure;

struct FileStreamAuth
{
	ServerConnection* caller = nullptr;
	hash_SHA256 code;
};

class User : public std::enable_shared_from_this<User>, public INetworkSender, public IUser
{
private:
	friend ServerConnection;
	friend class UsersManager;

	std::set<ServerConnection*> m_associatedConnections;
	ApoapseServer* server;
	Username m_username;
	DbId m_databaseId = -1;
	bool m_isUsingTemporaryPassword = false;
	const Usergroup* m_usergroup;
	std::optional<FileStreamAuth> m_fileStreamAuthCode;

	static constexpr UInt32 passwordAlgorithmIterations = 5000;

public:
	enum class UserStatus
	{
		offine = 0,
		online = 1
	};
	
	User(DataStructure& data, ServerConnection* connection, ApoapseServer* apoapseServer);
	virtual ~User() override;

	const Username& GetUsername() const override;
	const Usergroup& GetUsergroup() const override;
	
	// INetworkSender
	virtual void Send(BytesWrapper bytesPtr, TCPConnection* excludedConnection = nullptr) override;
	virtual void Send(StrWrapper strPtr, TCPConnection* excludedConnection = nullptr) override;
	virtual void Send(std::shared_ptr<class NetworkPayload> data, TCPConnection* excludedConnection = nullptr) override;

	virtual std::string GetEndpointStr() const override;
	virtual void Close() override;
	void RequestClose() override;
	// ~INetworkSender

	bool IsUsingTemporaryPassword() const override;
	FileStreamAuth GenerateFileStreamAuthCode(ServerConnection& caller);
	bool AuthenticateFileStream(const hash_SHA256& authCode, ServerFileStreamConnection* fileStream);

	static std::vector<byte> GenerateRandomSalt();
	static std::vector<byte> HashPassword(const std::vector<byte>& encryptedPassword, const std::vector<byte>& salt);
	static bool ComparePasswords(const std::vector<byte>& password, const std::vector<byte>& storedPassword, const std::vector<byte>& salt);
	static bool IsEncryptedPasswordValid(const std::vector<byte>& encryptedPassword);
	std::string GetNickname() const;

	static std::string GetAvatarFilePath(const Username& username);

private:
 	void RemoveConnection(ServerConnection* connection);
	void AddConnection(ServerConnection* connection);
 	std::shared_ptr<User> GetObjectShared();
};