#include "stdafx.h"
#include "UsersManager.h"
#include "Common.h"
#include "LocalUser.h"
#include "ApoapseServer.h"

UsersManager::UsersManager(ApoapseServer& apoapseServer) : server(apoapseServer)
{

}

UsersManager::~UsersManager()
{

}

std::shared_ptr<LocalUser> UsersManager::Authenticate(const ApoapseAddress::UsernameHash& usernamehash)
{
	SQLQuery query(server.database);
	query << SELECT << "id" << FROM << "local_users" << WHERE << "username" << EQUALS << usernamehash.GetStr();
	auto result = query.Exec();

	if (result.RowCount() != 1)
		return nullptr;
	
	const Int64 id = result[0][0].GetInt64();

	// #TODO Implement password check

	if (IsConnected(usernamehash))
		return GetConnectedUser(usernamehash).GetShared();					// User is already connected on another socket: return a new shared_ptr instance to the existing user
	else
		return std::make_shared<LocalUser>(*this, usernamehash, id);		// Create a new user
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

DbId UsersManager::TryGetUser(const ApoapseAddress::UsernameHash& username)
{
	SQLQuery query(server.database);
	query << SELECT << "id" << FROM << "local_users" << WHERE << "username" << EQUALS << username.GetStr();
	auto result = query.Exec();

	if (result.RowCount() != 1)
		return 0;
	else
		return result[0][0].GetInt64();
}

ApoapseAddress::ServerDomain UsersManager::GetCurrentServerDomain()
{
	return ApoapseAddress::ServerDomain(global->settings->ReadConfigValue_string("server.domain"));
}
