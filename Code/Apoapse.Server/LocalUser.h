#pragma once
#include <set>
#include "ClientConnection.h"
#include "ApoapseAddress.h"
#include "UsersManager.h"

class GenericConnection;
class Command;

class LocalUser : public std::enable_shared_from_this<LocalUser>
{
	friend class ClientConnection;

	UsersManager& m_usersManager;
	std::set<ClientConnection*> m_connections;
	ApoapseAddress::UsernameHash m_usernameHash;
	//TCP_IP_ADDRESS (hold a copy)

public:
	LocalUser(UsersManager& usersManager, const ApoapseAddress::UsernameHash& usernameHash);
	virtual ~LocalUser();

	std::shared_ptr<LocalUser> GetPtr();
	ApoapseAddress::UsernameHash GetUsernameHash() const;
	
private:
	void AssociateConnection(ClientConnection* connection);
	void RemoveAssociatedConnection(ClientConnection* connection);
};