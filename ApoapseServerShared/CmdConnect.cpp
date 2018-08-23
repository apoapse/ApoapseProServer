#include "stdafx.h"
#include "Common.h"
#include "Command.h"
#include "CommandsManager.h"
#include "ServerConnection.h"
#include "CryptographyTypes.hpp"
#include "GlobalVarDefines.hpp"
#include "SecurityAlert.h"
#include "UsersManager.h"
#include "ApoapseServer.h"
#include "CmdServerInfo.h"

class CmdConnect final : public Command
{
public:
	CommandInfo& CmdConnect::GetInfo() const override
	{
		static auto info = CommandInfo();
		info.command = CommandId::connect;
		info.serverOnly = true;
		info.onlyNonAuthenticated = true;
		info.fields =
		{
			Field{ "protocol_version", FieldRequirement::any_mendatory, FIELD_VALUE(int) },
			Field{ "username", FieldRequirement::any_mendatory, FIELD_VALUE_VALIDATOR(std::vector<byte>, Username::IsValid) },
			Field{ "password", FieldRequirement::any_mendatory, FIELD_VALUE_VALIDATOR(std::vector<byte>, [&](const auto& hash) { return (hash.size() == sha256Length); }) },
		};

		return info;
	}

	void Process(ServerConnection& sender) override
	{
		if (GetFieldsData().GetValue<int>("protocol_version") != protocolVersion)
		{
			SecurityLog::LogAlert(ApoapseErrorCode::protocol_version_not_supported, sender);
			return;
		}

		if (sender.server.usersManager->GetRegisteredUsersCount() == 0)
		{
			LOG << "No users and usergroups registered: in first setup state";
			CmdServerInfo cmd;
			cmd.SendSetupState(sender);
		}
		else
		{
			const auto username = Username(GetFieldsData().GetValue<std::vector<byte>>("username"));

			if (UsersManager::LoginIsValid(username, GetFieldsData().GetValue<std::vector<byte>>("password")))
			{
				sender.Authenticate(username);

				CmdServerInfo cmd;
				cmd.SendWellcome(sender, *sender.GetRelatedUser());
			}
			else
			{
				SecurityLog::LogAlert(ApoapseErrorCode::unable_to_authenticate_user, sender);
			}
		}
	}
};

APOAPSE_COMMAND_REGISTER(CmdConnect, CommandId::connect);