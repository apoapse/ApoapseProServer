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

public:
	User(DbId databaseId, const Username& username, const Uuid& usergroupUuid, ServerConnection* connection, ApoapseServer* apoapseServer);
	virtual ~User() override;

	const Username& GetUsername() const;
	const Uuid& GetUsergroup() const;
	void SetUsergroup(const Uuid& usergroupUuid);
	
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