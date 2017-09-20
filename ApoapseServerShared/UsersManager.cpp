#include "stdafx.h"
#include "UsersManager.h"
#include "Common.h"

bool UsersManager::IsUserConnected(const User::Address& address) const
{
	return (m_connectedUsers.count(address) > 0);
}

std::weak_ptr<User> UsersManager::GetUserByAddress(const User::Address& address) const
{
	try
	{
		auto* userRawPtr = m_connectedUsers.at(address);
		return userRawPtr->GetObjectShared();
	}
	catch (const std::exception&)
	{
		throw std::exception("The requested user does not exist");
	}
}

void UsersManager::RemoveConnectedUser(User& user)
{
	m_connectedUsers.erase(user.GetAddress());
}

void UsersManager::AddConnectedUser(User* user)
{
	m_connectedUsers[user->GetAddress()] = user;
}
