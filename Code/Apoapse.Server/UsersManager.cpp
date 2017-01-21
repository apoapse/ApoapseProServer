#include "stdafx.h"
#include "UsersManager.h"
#include "Common.h"
#include "LocalUser.h"

UsersManager::UsersManager()
{

}

UsersManager::~UsersManager()
{

}

void UsersManager::AddConnectedUser(LocalUser* user)
{
	if (user == nullptr)
		return;

	m_connectedUsers[user->GetUsernameHash()] = user;
}

void UsersManager::RemoveConnectedUser(LocalUser* user)
{
	if (user == nullptr)
		return;

	ASSERT(IsConnected(user->GetUsernameHash()));

	m_connectedUsers.erase(user->GetUsernameHash());
}

LocalUser& UsersManager::GetConnectedUser(const ApoapseAddress::UsernameHash& usernameHash)
{
	ASSERT(IsConnected(usernameHash));

	return *m_connectedUsers[usernameHash];
}

bool UsersManager::IsConnected(const ApoapseAddress::UsernameHash& usernameHash)
{
	return (m_connectedUsers.count(usernameHash) > 0);
}
