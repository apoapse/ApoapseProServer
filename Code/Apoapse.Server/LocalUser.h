#pragma once
#include <set>
#include "ClientConnection.h"
class GenericConnection;
class Command;

class LocalUser
{
	std::set<ClientConnection*> m_connections;

public:
	LocalUser(/*const Address& address*/);
	virtual ~LocalUser();

	void AssociateConnection(ClientConnection* connection);
	void RemoveAssociatedConnection(ClientConnection* connection);
	
private:
};