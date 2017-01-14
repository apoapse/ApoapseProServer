#pragma once
#include <set>
#include "ReadWriteMutexWrapper.h"
#include "LocalUser.h"

class ConnectedUsersManager
{
	ReadWriteMutexWrapper<std::set<LocalUser*>> m_connectedUsers;

public:
	ConnectedUsersManager();
	virtual ~ConnectedUsersManager();

	//bool IsUserConnected(username) const;
	//LocalUser& GetUser();
	void AddUser(LocalUser* user);
	void RemoveUser(LocalUser* user);
	
private:
};