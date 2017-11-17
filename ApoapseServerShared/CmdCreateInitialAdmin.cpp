#include "stdafx.h"
#include "Common.h"
#include "Command.h"
#include "CommandsManager.h"
#include "UsergroupsManager.h"
#include "UsersManager.h"
#include "ServerConnection.h"
#include "ApoapseServer.h"
#include "MemoryUtils.hpp"
#include "CryptographyTypes.hpp"

class CmdCreateInitialAdmin : public Command
{
public:
	CommandInfo& GetInfo() const override
	{
		static auto info = CommandInfo();
		info.command = CommandId::create_initial_admin;
		info.serverOnly = true;
		info.onlyNonAuthenticated = true;
		info.fields =
		{
			CommandField{ "usergroup.uuid", FieldRequirement::any_mendatory, FIELD_VALUE_VALIDATOR(ByteContainer, Uuid::IsValid) },
			CommandField{ "user.username", FieldRequirement::any_mendatory, FIELD_VALUE_VALIDATOR(ByteContainer, Username::IsValid) },
			CommandField{ "user.password", FieldRequirement::any_mendatory, FIELD_VALUE_VALIDATOR(ByteContainer, User::IsEncryptedPasswordValid) },
			CommandField{ "user.public_key", FieldRequirement::any_mendatory, FIELD_VALUE_VALIDATOR(ByteContainer, CommandField::ContainerIsNotEmpty<ByteContainer>) },
			CommandField{ "user.encrypted_private_key", FieldRequirement::any_mendatory, FIELD_VALUE_VALIDATOR(ByteContainer, CommandField::ContainerIsNotEmpty<ByteContainer>) },
		};

		return info;
	}

	void Process(ServerConnection& sender) override
	{
		if (sender.server.usergroupsManager->GetUsegroupsCount() == 0 && sender.server.usersManager->GetRegisteredUsersCount() == 0)
		{
			const Uuid usergroupUuid = Uuid(GetFieldsData().GetValue<ByteContainer>("usergroup.uuid"));
			static std::vector<std::string> permissions
			{
				"CREATE_USER",
				"CHANGE_PERMISSIONS",
				"CREATE_USERGROUP",
				"CHANGE_USERGROUP"
			};

			const Username username = Username(GetFieldsData().GetValue<ByteContainer>("user.username"));
			const PublicKeyBytes userPublicKey = GetFieldsData().GetValue<ByteContainer>("user.public_key");
			const EncryptedPrivateKeyBytes userEncryptedKey = GetFieldsData().GetValue<ByteContainer>("user.encrypted_private_key");

			sender.server.usersManager->RegisterNewUser(username, GetFieldsData().GetValue<ByteContainer>("user.password"));
			sender.server.usersManager->SetUserIdentity(username, GetFieldsData().GetValue<ByteContainer>("user.password"), userPublicKey, userEncryptedKey);

			if (!sender.server.usergroupsManager->TryCreateNewUsergroup(usergroupUuid, permissions, GetFieldsData()))
			{
				LOG << LogSeverity::error << "Error while trying to create a new usergroup";
				// #TODO Remove the user just created

				sender.Close(); // #TODO sec alert
				return;
			}

			LOG << "Administrator user and usergroup added";
			sender.Close();
		}
		else
		{
			LOG << LogSeverity::error << "Trying to create a new usergroup from an un-authenticated connection but the server is not in setup state";
		}
	}
	
private:
};