#pragma once
#include <map>
#include "ReadWriteMutexWrapper.h"
#include "ApoapseAddress.h"
class LocalUser;
class ApoapseServer;

class UsersManager
{
	//ReadWriteMutexWrapper<std::set<LocalUser*>> m_connectedUsers;
	std::map<ApoapseAddress::UsernameHash, LocalUser*> m_connectedUsers;

public:
	ApoapseServer& server;

	UsersManager(ApoapseServer& apoapseServer);
	virtual ~UsersManager();

	std::shared_ptr<LocalUser> Authenticate(const ApoapseAddress::UsernameHash& username);

	void AddConnectedUser(LocalUser* user);
	void RemoveConnectedUser(LocalUser* user);
	LocalUser& GetConnectedUser(const ApoapseAddress::UsernameHash& usernameHash);
	bool IsConnected(const ApoapseAddress::UsernameHash& usernameHash);

	//************************************
	// Method:    UsersManager::UserExist - Check if the specified user is registered on the current server and if possible returns the user's DbId
	// Access:    public 
	// Returns:   DbId - Return 0 if the user does not exist
	// Parameter: const ApoapseAddress::UsernameHash & username
	//************************************
	DbId TryGetUser(const ApoapseAddress::UsernameHash& username);

	static ApoapseAddress::ServerDomain GetCurrentServerDomain();
	
private:
};