#pragma once
#include <set>
#include "ClientConnection.h"
#include "ApoapseAddress.h"
#include "UsersManager.h"
#include "INetworkSender.h"

class GenericConnection;
class Command;

class LocalUser : public std::enable_shared_from_this<LocalUser>, public INetworkSender
{
	friend class ClientConnection;

	UsersManager& m_usersManager;
	std::set<ClientConnection*> m_connections;
	ApoapseAddress::UsernameHash m_usernameHash;
	//TCP_IP_ADDRESS (hold a copy)

public:
	ApoapseServer& server;

	LocalUser(ApoapseServer& apoapseServer, UsersManager& usersManager, const ApoapseAddress::UsernameHash& usernameHash);
	virtual ~LocalUser();

	std::shared_ptr<LocalUser> GetPtr();
	ApoapseAddress::UsernameHash GetUsernameHash() const;

	void Send(const std::vector<byte>& bytes);
	void Send(const std::string& str);
	
private:
	void AssociateConnection(ClientConnection* connection);
	void RemoveAssociatedConnection(ClientConnection* connection);
};