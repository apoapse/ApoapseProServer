#pragma once
#include <set>
#include "ClientConnection.h"
#include "ApoapseAddress.h"
#include "UsersManager.h"
#include "INetworkSender.h"

class TCPConnection;
class GenericConnection;
class Command;

class LocalUser : public std::enable_shared_from_this<LocalUser>, public INetworkSender
{
	friend class ClientConnection;

	UsersManager& m_usersManager;
	std::set<ClientConnection*> m_connections;
	const ApoapseAddress::UsernameHash m_usernameHash;
	const DbId m_id;
	//TCP_IP_ADDRESS (hold a copy)

public:
	LocalUser(UsersManager& usersManager, const ApoapseAddress::UsernameHash& usernameHash, DbId id);
	virtual ~LocalUser();

	std::shared_ptr<LocalUser> GetShared();
	ApoapseAddress::UsernameHash GetUsernameHash() const;
	ApoapseAddress GetFullAddress() const;

	void Send(std::shared_ptr<std::vector<byte>> bytesPtr, TCPConnection* excludedConnection = nullptr);
	void Send(std::unique_ptr<std::string> strPtr, TCPConnection* excludedConnection = nullptr);

	DbId GetDatabaseId() const;
	size_t GetConnectionsCount() const;

	//************************************
	// Method:    LocalUser::Disconnect - Close all the associated connections
	// Access:    public
	// Returns:   void
	//************************************
	void Disconnect();
	
private:
	void AssociateConnection(ClientConnection* connection);
	void RemoveAssociatedConnection(ClientConnection* connection);
};