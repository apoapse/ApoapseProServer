#pragma once
#include <map>
#include "HashTypes.hpp"
#include "User.h"
class ServerConnection;

class UsersManager
{
	friend class User;
	friend class ServerConnection;
	std::map<User::Username, User*> m_connectedUsers; // #THREADING

public:


// 	UsersManager();
// 	virtual ~UsersManager();

	
	bool IsUserConnected(const User::Username& username) const;
	std::weak_ptr<User> GetUserByName(const User::Username& username) const;

	std::shared_ptr<User> CreateUserObject(const User::Username& username, ServerConnection& connection);
	static bool LoginIsValid(const User::Username& username, const hash_SHA3_256& password);

private:
	void RemoveConnectedUser(User& user);
	void AddConnectedUser(User* user);
};