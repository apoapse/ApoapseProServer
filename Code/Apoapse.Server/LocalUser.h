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
	const ApoapseAddress::UsernameHash m_usernameHash;
	const Int64 m_id;
	//TCP_IP_ADDRESS (hold a copy)

public:
	LocalUser(UsersManager& usersManager, const ApoapseAddress::UsernameHash& usernameHash, Int64 id);
	virtual ~LocalUser();

	std::shared_ptr<LocalUser> GetShared();
	ApoapseAddress::UsernameHash GetUsernameHash() const;

	void Send(const std::vector<byte>& bytes);
	void Send(const std::string& str);
	
private:
	void AssociateConnection(ClientConnection* connection);
	void RemoveAssociatedConnection(ClientConnection* connection);
};