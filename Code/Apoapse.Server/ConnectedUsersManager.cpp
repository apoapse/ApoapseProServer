#include "stdafx.h"
#include "ConnectedUsersManager.h"
#include "Common.h"

ConnectedUsersManager::ConnectedUsersManager()
{

}

ConnectedUsersManager::~ConnectedUsersManager()
{

}

void ConnectedUsersManager::AddUser(LocalUser* user)
{
	if (user == nullptr)
		return;

	exclusive_mutex lock(m_connectedUsers.GetMutex());
	m_connectedUsers->insert(user);
}

void ConnectedUsersManager::RemoveUser(LocalUser* user)
{
	if (user == nullptr)
		return;

	exclusive_mutex lock(m_connectedUsers.GetMutex());
	m_connectedUsers->erase(user);
}
