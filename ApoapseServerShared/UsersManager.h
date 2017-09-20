#pragma once
#include <map>
#include "User.h"

class UsersManager
{
	friend class User;
	friend class ServerConnection;
	std::map<User::Address, User*> m_connectedUsers; // #THREADING

public:


// 	UsersManager();
// 	virtual ~UsersManager();

	
	bool IsUserConnected(const User::Address& address) const;
	std::weak_ptr<User> GetUserByAddress(const User::Address& address) const;
private:
	void RemoveConnectedUser(User& user);
	void AddConnectedUser(User* user);
};