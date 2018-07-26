#include "stdafx.h"
#include "UsersManager.h"
#include "Common.h"
#include "SQLQuery.h"
#include "ServerConnection.h"
#include "ApoapseServer.h"

bool UsersManager::IsUserConnected(const Username& username) const
{
	return (m_connectedUsers.count(username) > 0);
}

std::weak_ptr<User> UsersManager::GetUserByUsername(const Username& username) const
{
	try
	{
		auto* userRawPtr = m_connectedUsers.at(username);
		return userRawPtr->GetObjectShared();
	}
	catch (const std::exception&)
	{
		throw std::exception("The requested user does not exist");
	}
}

size_t UsersManager::GetConnectedUsersCount() const
{
	return m_connectedUsers.size();
}

size_t UsersManager::GetRegisteredUsersCount() const
{
	SQLQuery query(*global->database);
	query << SELECT << "username_hash" << FROM << "users";
	auto res = query.Exec();

	return (res) ? res.RowCount() : 0;
}

std::shared_ptr<User> UsersManager::CreateUserObject(const Username& username, ServerConnection& connection)
{
	SQLQuery query(*global->database);
	query << SELECT << "user_id,identity_key_public" << FROM << "users" << WHERE << "username_hash" << EQUALS << username.GetRaw();
	auto res = query.Exec();

	if (!res || res.RowCount() != 1)
		throw std::exception("Unable to find the user on the database");

	const auto user_id = res[0][0].GetInt64();
	const PublicKeyBytes identityPublicKey = res[0][1].GetByteArray();
	//const Uuid usergroupUuid = connection.server.usergroupsManager->GetUsergroupOfUser(username).uuid;

	auto userPtr = std::make_shared<User>(user_id, username, &connection, &connection.server);
	AddConnectedUser(userPtr.get());

	return userPtr;
}

bool UsersManager::LoginIsValid(const Username& username, const std::vector<byte>& password)
{
	SQLQuery query(*global->database);
	query << SELECT << "password,password_salt" << FROM << "users" << WHERE << "username_hash" << EQUALS << username.GetRaw();
	auto res = query.Exec();

	if (!res || res.RowCount() == 0)
		return false;

	const auto dbPassword = res[0][0].GetByteArray();
	const auto dbPassword_salt = res[0][1].GetByteArray();

	return User::ComparePasswords(password, dbPassword, dbPassword_salt);
}

bool UsersManager::DoesUserExist(const Username& username) const
{
	SQLQuery query(*global->database);
	query << SELECT << "username_hash" << FROM << "users" << WHERE << "username_hash" << EQUALS << username.GetRaw();
	auto res = query.Exec();

	if (res && res.RowCount() == 1)
		return true;
	else
		return false;
}

void UsersManager::RegisterNewUser(const Username& username, const std::vector<byte>& encryptedTemporaryPassword)
{
	SECURITY_ASSERT(!DoesUserExist(username));

	const auto salt = User::GenerateRandomSalt();

	SQLQuery query(*global->database);
	query << INSERT_INTO << "users" << " (username_hash, password, password_salt)" << VALUES << "(" << username.GetRaw() << "," << User::HashPassword(encryptedTemporaryPassword, salt) << "," << salt << ")";

	query.Exec();
}

void UsersManager::SetUserIdentity(const Username& username, const std::vector<byte>& encryptedPassword, const PublicKeyBytes& identityKey, const EncryptedPrivateKeyBytes& identityPrivateKey, const IV& identityIV)
{
	SECURITY_ASSERT(DoesUserExist(username));

	const auto salt = User::GenerateRandomSalt();
	const auto passwordForStorage = User::HashPassword(encryptedPassword, salt);

	SQLQuery query(*global->database);
	query << UPDATE << "users" << SET << "password=" << passwordForStorage << ",password_salt=" << salt << ",identity_key_public=" << identityKey << ",identity_key_private_encrypted=" << identityPrivateKey
		<< ",identity_key_iv=" << identityIV << WHERE "username_hash" << EQUALS << username.GetRaw();
	query.Exec();
}

void UsersManager::RemoveConnectedUser(User& user)
{
	m_connectedUsers.erase(user.GetUsername());
}

void UsersManager::AddConnectedUser(User* user)
{
	m_connectedUsers[user->GetUsername()] = user;
}
