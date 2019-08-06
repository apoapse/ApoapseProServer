#include "stdafx.h"
#include "Common.h"
#include "Command.h"
#include "CommandsManager.h"
#include "ServerConnection.h"
#include "CryptographyTypes.hpp"
#include "ApoapseServer.h"
#include "Username.h"
#include "UsersManager.h"
#include "SecurityAlert.h"
#include "User.h"

class CmdRegisterNewUser final : public Command
{
public:
	CommandInfo& CmdRegisterNewUser::GetInfo() const override
	{
		static auto info = CommandInfo();
		info.command = CommandId::register_new_user;
		info.serverOnly = true;
		info.requireAuthentication = true;
		info.fields =
		{
			Field{ "username", FieldRequirement::any_mendatory, FIELD_VALUE_VALIDATOR(std::vector<byte>, Username::IsValid) },
			Field{ "temporary_password", FieldRequirement::any_mendatory, FIELD_VALUE_VALIDATOR(std::vector<byte>, [&](const auto& hash) { return (hash.size() == sha256Length); }) },
		};

		return info;
	}

	void Process(User& sender, ServerConnection& senderConnection) override
	{
		const auto username = Username(GetFieldsData().GetValue<std::vector<byte>>("username"));
		const auto password = GetFieldsData().GetValue<std::vector<byte>>("temporary_password");

		if (!senderConnection.server.usersManager->DoesUserExist(username))
		{
//			senderConnection.server.usersManager->RegisterNewUser(username, password);
		}
		else
		{
			SecurityLog::LogAlert(ApoapseErrorCode::unable_to_register_user, sender);
		}
	}
};

APOAPSE_COMMAND_REGISTER(CmdRegisterNewUser, CommandId::register_new_user);