#pragma once
#include <map>
#include "CryptographyTypes.hpp"
#include "User.h"
#include "Uuid.h"
class ServerConnection;

class UsersManager
{
	friend class User;
	friend class ServerConnection;
	std::map<Username, User*> m_connectedUsers; // #THREADING

public:


// 	UsersManager();
// 	virtual ~UsersManager();

	
	bool IsUserConnected(const Username& username) const;
	std::weak_ptr<User> GetUserByUsername(const Username& username) const;
	size_t GetConnectedUsersCount() const;
	size_t GetRegisteredUsersCount() const;

	std::shared_ptr<User> CreateUserObject(const Username& username, ServerConnection& connection);
	static bool LoginIsValid(const Username& username, const std::vector<byte>& password);

	bool DoesUserExist(const Username& username) const;

	void RegisterNewUser(const Username& username, const std::vector<byte>& encryptedTemporaryPassword);
	void SetUserIdentity(const Username& username, const std::vector<byte>& encryptedPassword, const PublicKeyBytes& identityKey, const EncryptedPrivateKeyBytes& identityPrivateKey, const IV& identityIV);

private:
	void RemoveConnectedUser(User& user);
	void AddConnectedUser(User* user);
};