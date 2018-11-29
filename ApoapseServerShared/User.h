#pragma once
#include "INetworkSender.h"
#include <set>
#include "CryptographyTypes.hpp"
#include "Username.h"
#include "ApoapseMetadata.h"
class ServerConnection;
class ApoapseServer;

class User : public std::enable_shared_from_this<User>, public INetworkSender
{
private:
	friend ServerConnection;
	friend class UsersManager;

	std::set<ServerConnection*> m_associatedConnections;
	ApoapseServer* server;
	Username m_username;
	DbId m_databaseId = 0;

	static constexpr UInt32 passwordAlgorithmIterations = 5000;

public:
	User(DbId databaseId, const Username& username, ServerConnection* connection, ApoapseServer* apoapseServer);
	virtual ~User() override;

	const Username& GetUsername() const;
	
	// INetworkSender
	virtual void Send(BytesWrapper bytesPtr, TCPConnection* excludedConnection = nullptr) override;
	virtual void Send(StrWrapper strPtr, TCPConnection* excludedConnection = nullptr) override;
	virtual void Send(std::shared_ptr<class NetworkPayload> data, TCPConnection* excludedConnection = nullptr) override;

	virtual std::string GetEndpointStr() const override;
	virtual void Close() override;
	// ~INetworkSender

	ApoapseMetadata GetMetadata(MetadataAcess type) const;

	bool IsUsingTemporaryPassword() const;

	static std::vector<byte> GenerateRandomSalt();
	static std::vector<byte> HashPassword(const std::vector<byte>& encryptedPassword, const std::vector<byte>& salt);
	static bool ComparePasswords(const std::vector<byte>& password, const std::vector<byte>& storedPassword, const std::vector<byte>& salt);
	static bool IsEncryptedPasswordValid(const std::vector<byte>& encryptedPassword);

private:
 	void RemoveConnection(ServerConnection* connection);
	void AddConnection(ServerConnection* connection);
 	std::shared_ptr<User> GetObjectShared();
};