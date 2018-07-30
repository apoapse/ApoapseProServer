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

class CmdRegisterNewUser final : public Command
{
public:
	CommandInfo& CmdRegisterNewUser::GetInfo() const override
	{
		static auto info = CommandInfo();
		info.command = CommandId::register_new_user;
		info.serverOnly = true;
		//info.requireAuthentication = true; // #MVP
		info.fields =
		{
			CommandField{ "username", FieldRequirement::any_mendatory, FIELD_VALUE_VALIDATOR(std::vector<byte>, Username::IsValid) },
			CommandField{ "temporary_password", FieldRequirement::any_mendatory, FIELD_VALUE_VALIDATOR(std::vector<byte>, [&](const auto& hash) { return (hash.size() == sha256Length); }) },
		};

		return info;
	}

	//void Process(User& sender, ServerConnection& senderConnection) override #MVP

	void Process(ServerConnection& sender) override
	{
		
		const auto username = Username(GetFieldsData().GetValue<std::vector<byte>>("username"));
		const auto password = GetFieldsData().GetValue<std::vector<byte>>("temporary_password");

		if (!sender.server.usersManager->DoesUserExist(username))
		{
			sender.server.usersManager->RegisterNewUser(username, password);
		}
		else
		{
			SecurityLog::LogAlert(ApoapseErrorCode::unable_to_register_user, sender);
		}
	}
};

APOAPSE_COMMAND_REGISTER(CmdRegisterNewUser, CommandId::register_new_user);