#include "stdafx.h"
#include "Common.h"
#include "ServerCmdManager.h"
#include "ServerConnection.h"
#include "CommandsDef.hpp"
#include <SecurityAlert.h>
#include "GlobalVarDefines.hpp"
#include "ApoapseServer.h"
#include "UsersManager.h"

ServerCmdManager::ServerCmdManager() : CommandsManagerV2(GetCommandDef())
{
	
}

bool ServerCmdManager::OnReceivedCommandPre(CommandV2& cmd, GenericConnection& netConnection)
{
	auto& data = cmd.GetData();

	if (cmd.name == "login")
	{
		if (data.GetField("protocol_version").GetValue<Int64>() != protocolVersion)
		{
			SecurityLog::LogAlert(ApoapseErrorCode::protocol_version_not_supported, netConnection);
			return false;
		}
	}

	return true;
}

void ServerCmdManager::OnReceivedCommand(CommandV2& cmd, GenericConnection& netConnection)
{
	auto& connection = static_cast<ServerConnection&>(netConnection);
	auto& data = cmd.GetData();

	if (cmd.name == "login")
	{
		const Username username = data.GetField("username").GetValue<Username>();
		auto dataStruct = global->apoapseData->GetStructure("server_info");

		dataStruct.GetField("username").SetValue(username);

		if (connection.server.usersManager->GetRegisteredUsersCount() == 0)
		{
			LOG << "No users and usergroups registered: in first setup state";

			dataStruct.GetField("status").SetValue("setup_state");
		}
		else
		{
			if (UsersManager::LoginIsValid(username, data.GetField("password").GetValue<ByteContainer>()))
			{
				connection.Authenticate(username);
				
				dataStruct.GetField("status").SetValue("authenticated");
				dataStruct.GetField("requirePasswordChange").SetValue(connection.GetRelatedUser()->IsUsingTemporaryPassword());
			}
			else
			{
				SecurityLog::LogAlert(ApoapseErrorCode::unable_to_authenticate_user, connection);
				return;
			}
		}

		global->cmdManager->CreateCommand("server_info", dataStruct).Send(connection);
	}

	LOG_DEBUG << "RECEIVED!";
}
