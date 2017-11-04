#pragma once
#include <map>
#include "CryptographyTypes.hpp"
#include "User.h"
#include "IUsersManager.h"
class ServerConnection;

class UsersManager: public IUsersManager
{
	friend class User;
	friend class ServerConnection;
	std::map<Username, User*> m_connectedUsers; // #THREADING

public:


// 	UsersManager();
// 	virtual ~UsersManager();

	
	bool IsUserConnected(const Username& username) const;
	std::weak_ptr<User> GetUserByUsername(const Username& username) const;

	std::shared_ptr<User> CreateUserObject(const Username& username, ServerConnection& connection);
	static bool LoginIsValid(const Username& username, const hash_SHA3_256& password);


	bool DoesUserExist(const Username& username) const override;
	PublicKeyBytes GetUserIdentityPublicKey(const Username& username) const override;

private:
	void RemoveConnectedUser(User& user);
	void AddConnectedUser(User* user);
};