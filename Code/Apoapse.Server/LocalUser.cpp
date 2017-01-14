#include "stdafx.h"
#include "LocalUser.h"
#include "Common.h"

LocalUser::LocalUser(/*const Address& address*/)
{

}

LocalUser::~LocalUser()
{
	LOG_DEBUG_FUNCTION_NAME();
}

void LocalUser::AssociateConnection(ClientConnection* connection)
{
	m_connections.insert(connection);
}

void LocalUser::RemoveAssociatedConnection(ClientConnection* connection)
{
	m_connections.erase(connection);
}