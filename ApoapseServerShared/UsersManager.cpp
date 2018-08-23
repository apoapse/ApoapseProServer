#include "stdafx.h"
#include "UsersManager.h"
#include "Common.h"
#include "SQLQuery.h"
#include "ServerConnection.h"
#include "ApoapseServer.h"
#include "ApoapseMetadata.h"
#include "Operation.h"

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

	return ((res) ? res.RowCount() : 0);
}

std::shared_ptr<User> UsersManager::CreateUserObject(const Username& username, ServerConnection& connection)
{
	SQLQuery query(*global->database);
	query << SELECT << "user_id" << FROM << "users" << WHERE << "username_hash" << EQUALS << username.GetRaw();
	auto res = query.Exec();

	if (!res || res.RowCount() != 1)
		throw std::exception("Unable to find the user on the database");

	const auto user_id = res[0][0].GetInt64();
	//const PublicKeyBytes identityPublicKey = res[0][1].GetByteArray();
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
	query << INSERT_INTO << "users" << " (username_hash, password, password_salt, is_temporary_passsword)" << VALUES << "(" << username.GetRaw() << "," << User::HashPassword(encryptedTemporaryPassword, salt) << "," << salt << "," << 1 << ")";

	query.Exec();

	LOG << "User " << username.ToStr() << " registered";
}

void UsersManager::SetUserIdentity(const Username& username, const std::vector<byte>& encryptedPassword, const ApoapseMetadata& metadataAll/*, const PublicKeyBytes& identityKey, const EncryptedPrivateKeyBytes& identityPrivateKey, const IV& identityIV*/)
{
	SECURITY_ASSERT(DoesUserExist(username));

	const auto salt = User::GenerateRandomSalt();
	const auto passwordForStorage = User::HashPassword(encryptedPassword, salt);

	{
		SQLQuery query(*global->database);
		query << UPDATE << "users" << SET << "password=" << passwordForStorage << ",password_salt=" << salt << ",is_temporary_passsword=" << 0 << ",metadata_all=" << metadataAll.GetRawDataForDb() << WHERE "username_hash" << EQUALS << username.GetRaw();
		query.Exec();
	}

	{
		SQLQuery query(*global->database);
		query << SELECT << "user_id" << FROM "users" << WHERE "username_hash" << EQUALS << username.GetRaw();
		auto res = query.Exec();

		Operation(OperationType::new_user, username, res[0][0].GetInt64()).Save();
	}
}

void UsersManager::Send(BytesWrapper bytesPtr, TCPConnection* excludedConnection /*= nullptr*/)
{
	for (const auto& user : m_connectedUsers)
	{
		user.second->Send(bytesPtr, excludedConnection);
	}
}

void UsersManager::Send(StrWrapper strPtr, TCPConnection* excludedConnection /*= nullptr*/)
{
	for (const auto& user : m_connectedUsers)
	{
		user.second->Send(std::move(strPtr), excludedConnection);
	}
}

void UsersManager::Send(std::shared_ptr<class NetworkPayload> data, TCPConnection* excludedConnection /*= nullptr*/)
{
	for (const auto& user : m_connectedUsers)
	{
		user.second->Send(data, excludedConnection);
	}
}

std::string UsersManager::GetEndpointStr() const
{
	return "All connected users (" + std::to_string(m_connectedUsers.size()) + " connected)";
}

void UsersManager::Close()
{
	for (const auto& user : m_connectedUsers)
	{
		user.second->Close();
	}
}

void UsersManager::RemoveConnectedUser(User& user)
{
	m_connectedUsers.erase(user.GetUsername());
}

void UsersManager::AddConnectedUser(User* user)
{
	m_connectedUsers[user->GetUsername()] = user;
}
