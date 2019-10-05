#include "stdafx.h"
#include "UsersManager.h"
#include "Common.h"
#include "SQLQuery.h"
#include "ServerConnection.h"
#include "ApoapseServer.h"
#include "CommandsManagerV2.h"
#include "ApoapseOperation.h"

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
		throw std::runtime_error("The requested user does not exist");
	}
}

size_t UsersManager::GetConnectedUsersCount() const
{
	return m_connectedUsers.size();
}

size_t UsersManager::GetRegisteredUsersCount() const
{
	SQLQuery query(*global->database);
	query << SELECT << "username" << FROM << "users";
	auto res = query.Exec();

	return ((res) ? res.RowCount() : 0);
}

std::vector<User*> UsersManager::GetConnectedUsers() const
{
	std::vector<User*> output;
	output.reserve(m_connectedUsers.size());

	for (const auto& user : m_connectedUsers)
	{
		output.push_back(user.second);
	}
	
	return output;
}

std::shared_ptr<User> UsersManager::CreateUserObject(const Username& username, ServerConnection& connection)
{
	DataStructure dat = global->apoapseData->ReadItemFromDatabase("user", "username", username.GetBytes());

	auto userPtr = std::make_shared<User>(dat, &connection, &connection.server);
	AddConnectedUser(userPtr.get());

	return userPtr;
}

bool UsersManager::LoginIsValid(const Username& username, const std::vector<byte>& password)
{
	SQLQuery query(*global->database);
	query << SELECT << "password,password_salt" << FROM << "users" << WHERE << "username" << EQUALS << username.GetRaw();
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
	query << SELECT << "username" << FROM << "users" << WHERE << "username" << EQUALS << username.GetRaw();
	auto res = query.Exec();

	return (res && res.RowCount() == 1);
}

void UsersManager::RegisterNewUser(const Username& username, const std::vector<byte>& encryptedTemporaryPassword, const Uuid& usergroup)
{
	SECURITY_ASSERT(!DoesUserExist(username));

	const ByteContainer salt = User::GenerateRandomSalt();

	auto dat = global->apoapseData->GetStructure("user");
	dat.GetField("username").SetValue(username);
	dat.GetField("password").SetValue(User::HashPassword(encryptedTemporaryPassword, salt));
	dat.GetField("password_salt").SetValue(salt);
	dat.GetField("is_temporary_password").SetValue(true);
	dat.GetField("usergroup").SetValue(usergroup);
	dat.SaveToDatabase();

	LOG << "New user " << username.ToStr() << " registered";
}

DbId UsersManager::SetUserIdentity(const Username& username, const std::vector<byte>& encryptedPassword, const std::string& nickname/*, const PublicKeyBytes& identityKey, const EncryptedPrivateKeyBytes& identityPrivateKey, const IV& identityIV*/)
{
	SECURITY_ASSERT(DoesUserExist(username));

	const auto salt = User::GenerateRandomSalt();
	const auto passwordForStorage = User::HashPassword(encryptedPassword, salt);

	auto dat = global->apoapseData->GetStructure("user");
	dat.GetField("username").SetValue(username);
	dat.GetField("password").SetValue(passwordForStorage);
	dat.GetField("password_salt").SetValue(salt);
	dat.GetField("is_temporary_password").SetValue(false);
	dat.GetField("nickname").SetValue(nickname);
	dat.SaveToDatabase();

	auto cmd = global->cmdManager->CreateCommand("user", dat);
	ApoapseOperation::RegisterOperation(cmd, username, true);
	cmd.Send(*this);

	return -1;
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

void UsersManager::RequestClose()
{
	for (const auto& user : m_connectedUsers)
	{
		user.second->RequestClose();
	}
}

void UsersManager::SendUserStatusChange(const Username& username, User::UserStatus status)
{
	DataStructure dat = global->apoapseData->GetStructure("user_status");
	dat.GetField("user").SetValue(username);
	dat.GetField("status").SetValue(static_cast<Int64>(status));
	
	global->cmdManager->CreateCommand("change_user_status", dat).Send(*this);
}

void UsersManager::RemoveConnectedUser(User& user)
{
	const Username username = user.GetUsername();
	
	m_connectedUsers.erase(username);
	SendUserStatusChange(username, User::UserStatus::offine);
}

void UsersManager::AddConnectedUser(User* user)
{
	if (!user)
		return;

	const Username username = user->GetUsername();
	
	SendUserStatusChange(username, User::UserStatus::online);
	m_connectedUsers[username] = user;
}
