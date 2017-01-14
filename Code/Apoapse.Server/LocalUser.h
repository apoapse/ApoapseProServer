#pragma once
#include <set>
#include "ClientConnection.h"
class GenericConnection;
class Command;

class LocalUser
{
	std::set<ClientConnection*> m_connections;
	//TCP_IP_ADDRESS (hold a copy)

public:
	LocalUser(/*const Address::Username& address*/);
	virtual ~LocalUser();

	void AssociateConnection(ClientConnection* connection);
	void RemoveAssociatedConnection(ClientConnection* connection);
	
private:
};