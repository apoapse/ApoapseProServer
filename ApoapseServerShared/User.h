#pragma once
#include "INetworkSender.h"
#include <set>
class ServerConnection;
class ApoapseServer;

class User : public std::enable_shared_from_this<User>, public INetworkSender
{
public:
	class Address
	{
	public:
		Address();//TODODODOD

		std::array<byte, 32> username;	// SHA-256 hash

		bool operator<(const Address& other) const;
		bool operator==(const Address& other) const;

		std::string ToStr() const;

		friend std::ostream& operator<<(std::ostream& stream, const Address& address)
		{
			stream << address.ToStr();
			return stream;
		}
	};

private:
	friend ServerConnection;
	friend class UsersManager;

	std::set<ServerConnection*> m_associatedConnections;
	ApoapseServer* server;
	Address m_address;

public:
	User(const Address& address, ServerConnection* connection, ApoapseServer* apoapseServer);
	virtual ~User() override;

	Address GetAddress() const;
	
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