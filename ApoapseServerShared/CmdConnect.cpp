#include "stdafx.h"
#include "Common.h"
#include "Command.h"
#include "CommandsManager.h"
#include "ServerConnection.h"
#include "CryptographyTypes.hpp"
#include "ProtocolVersion.hpp"
#include "SecurityAlert.h"
#include "UsersManager.h"
#include "UsergroupsManager.h"
#include "ApoapseServer.h"
#include "CmdServerInfo.h"

class CmdConnect final : public Command
{
	CommandInfo& CmdConnect::GetInfo() const override
	{
		static auto info = CommandInfo();
		info.command = CommandId::connect;
		info.serverOnly = true;
		info.onlyNonAuthenticated = true;
		info.fields =
		{
			CommandField{ "protocol_version", FieldRequirement::any_mendatory, FIELD_VALUE(int) },
			CommandField{ "username", FieldRequirement::any_mendatory, FIELD_VALUE_VALIDATOR(std::vector<byte>, [&](const auto& hash) { return (hash.size() == sha256Length); }) },
			CommandField{ "password", FieldRequirement::any_mendatory, FIELD_VALUE_VALIDATOR(std::vector<byte>, [&](const auto& hash) { return (hash.size() == sha256Length); }) },
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

		if (sender.server.usersManager->GetRegisteredUsersCount() == 0 && sender.server.usergroupsManager->GetUsegroupsCount() == 0)
		{
			LOG << "No users and usergroups registered: first setup state";
			CmdServerInfo cmd;
			cmd.SendSetupState(sender);

			return;
		}

		auto username = Username(GetFieldsData().GetValue<std::vector<byte>>("username"));
		hash_SHA256 password;
		{
			auto passwordVector = GetFieldsData().GetValue<std::vector<byte>>("password");
			std::copy(passwordVector.begin(), passwordVector.end(), password.begin());
		}

		if (UsersManager::LoginIsValid(username, password))
		{
			sender.Authenticate(username);
			//SEND CMD
			LOG << "LoginIsValid";
		}
		else
		{
			LOG << "LoginIsNOTValid";
		}
	}
};

APOAPSE_COMMAND_REGISTER(CmdConnect, CommandId::connect);