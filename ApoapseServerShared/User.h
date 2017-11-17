#pragma once
#include "INetworkSender.h"
#include <set>
#include "CryptographyTypes.hpp"
#include "Username.h"
#include "Usergroup.h"
#include "IUser.hpp"
class ServerConnection;
class ApoapseServer;

class User : public std::enable_shared_from_this<User>, public INetworkSender, public IUser
{
private:
	friend ServerConnection;
	friend class UsersManager;

	std::set<ServerConnection*> m_associatedConnections;
	ApoapseServer* server;
	Username m_username;
	DbId m_databaseId = 0;
	Uuid m_usergroupUuid;

	static constexpr UInt32 passwordAlgorithmIterations = 5000;

public:
	User(DbId databaseId, const Username& username, const Uuid& usergroupUuid, ServerConnection* connection, ApoapseServer* apoapseServer);
	virtual ~User() override;

	const Username& GetUsername() const override;
	const Uuid& GetUsergroup() const override;
	void SetUsergroup(const Uuid& usergroupUuid) override;
	
	// INetworkSender
	virtual void Send(BytesWrapper bytesPtr, TCPConnection* excludedConnection = nullptr) override;
	virtual void Send(StrWrapper strPtr, TCPConnection* excludedConnection = nullptr) override;
	virtual void Send(std::unique_ptr<class NetworkPayload> data, TCPConnection* excludedConnection = nullptr) override;

	virtual std::string GetEndpointStr() const override;
	virtual void Close() override;

	static std::vector<byte> GenerateRandomSalt();
	static std::vector<byte> HashPassword(const std::vector<byte>& encryptedPassword, const std::vector<byte>& salt);
	static bool ComparePasswords(const std::vector<byte>& password, const std::vector<byte>& storedPassword, const std::vector<byte>& salt);
	static bool IsEncryptedPasswordValid(const std::vector<byte>& encryptedPassword);

private:
 	void RemoveConnection(ServerConnection* connection);
	void AddConnection(ServerConnection* connection);
 	std::shared_ptr<User> GetObjectShared();
};