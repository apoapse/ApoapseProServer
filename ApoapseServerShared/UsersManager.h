#pragma once
#include <map>
#include "CryptographyTypes.hpp"
#include "User.h"
#include "Uuid.h"
#include "INetworkSender.h"
class ServerConnection;
class ApoapseMetadata;

class UsersManager : public INetworkSender
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
	std::vector<User*> GetConnectedUsers() const;

	std::shared_ptr<User> CreateUserObject(const Username& username, ServerConnection& connection);
	static bool LoginIsValid(const Username& username, const std::vector<byte>& password);

	bool DoesUserExist(const Username& username) const;

	void RegisterNewUser(const Username& username, const std::vector<byte>& encryptedTemporaryPassword, const Uuid& usergroup);
	DbId SetUserIdentity(const Username& username, const std::vector<byte>& encryptedPassword, const std::string& nickname/*, const PublicKeyBytes& identityKey, const EncryptedPrivateKeyBytes& identityPrivateKey, const IV& identityIV*/);

	// INetworkSender -> In the users manager, used to send to all connecter users
	virtual void Send(BytesWrapper bytesPtr, TCPConnection* excludedConnection = nullptr) override;
	virtual void Send(StrWrapper strPtr, TCPConnection* excludedConnection = nullptr) override;
	virtual void Send(std::shared_ptr<class NetworkPayload> data, TCPConnection* excludedConnection = nullptr) override;

	virtual std::string GetEndpointStr() const override;
	virtual void Close() override;
	//~ INetworkSender

	void SendUserStatusChange(const Username& username, User::UserStatus status);

private:
	void RemoveConnectedUser(User& user);
	void AddConnectedUser(User* user);
};