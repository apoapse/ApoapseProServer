#pragma once
#include "INetworkSender.h"
#include <set>
#include "HashTypes.hpp"
#include "ByteUtils.hpp"
class ServerConnection;
class ApoapseServer;

class User : public std::enable_shared_from_this<User>, public INetworkSender
{
public:
	class Username
	{
		hash_SHA3_256 m_usernameHash;

	public:
		Username(const hash_SHA3_256& hash) : m_usernameHash(hash)
		{
		}

		Username(const std::vector<byte>& hash);

		const hash_SHA3_256& GetRaw() const
		{
			return m_usernameHash;
		}

		std::string ToStr() const
		{
			return BytesToHexString(m_usernameHash);
		}

		friend std::ostream& operator<<(std::ostream& stream, const Username& username)
		{
			stream << username.ToStr();
			return stream;
		}

		bool operator<(const Username& other) const
		{
			return (m_usernameHash < other.GetRaw());
		}

		bool operator==(const Username& other) const
		{
			return (m_usernameHash == other.GetRaw());
		}
	};

private:
	friend ServerConnection;
	friend class UsersManager;

	std::set<ServerConnection*> m_associatedConnections;
	ApoapseServer* server;
	Username m_username;
	DbId m_databaseId = 0;

public:
	User(DbId databaseId, const Username& username, ServerConnection* connection, ApoapseServer* apoapseServer);
	virtual ~User() override;

	const Username& GetUsername() const;
	
	// INetworkSender
	virtual void Send(BytesWrapper bytesPtr, TCPConnection* excludedConnection = nullptr) override;
	virtual void Send(StrWrapper strPtr, TCPConnection* excludedConnection = nullptr) override;
	virtual void Send(std::unique_ptr<class NetworkPayload> data, TCPConnection* excludedConnection = nullptr) override;

	virtual std::string GetEndpointStr() const override;
	virtual void Close() override;

private:
 	void RemoveConnection(ServerConnection* connection);
	void AddConnection(ServerConnection* connection);
 	std::shared_ptr<User> GetObjectShared();
};