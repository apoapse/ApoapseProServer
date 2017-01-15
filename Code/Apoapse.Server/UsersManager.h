#pragma once
//#include <set>
#include <map>
#include "ReadWriteMutexWrapper.h"
#include "ApoapseAddress.h"
class LocalUser;

class UsersManager
{
	//ReadWriteMutexWrapper<std::set<LocalUser*>> m_connectedUsers;
	std::map<ApoapseAddress::UsernameHash, LocalUser*> m_connectedUsers;

public:
	UsersManager();
	virtual ~UsersManager();

	//bool IsUserConnected(username) const;
	//LocalUser& GetUser();
	void AddConnectedUser(LocalUser* user);
	void RemoveConnectedUser(LocalUser* user);
	LocalUser& GetConnectedUser(const ApoapseAddress::UsernameHash& usernameHash);
	bool IsConnected(const ApoapseAddress::UsernameHash& usernameHash);
	
private:
};