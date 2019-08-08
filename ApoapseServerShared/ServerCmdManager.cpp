#include "stdafx.h"
#include "Common.h"
#include "ServerCmdManager.h"
#include "ServerConnection.h"
#include "CommandsDef.hpp"
#include <SecurityAlert.h>
#include "GlobalVarDefines.hpp"
#include "ApoapseServer.h"
#include "UsersManager.h"
#include "ApoapseOperation.h"
#include "UsergroupManager.h"

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
	auto& connection = dynamic_cast<ServerConnection&>(netConnection);
	auto& data = cmd.GetData();

	if (cmd.name == "login")
	{
		const Username username = data.GetField("username").GetValue<Username>();
		auto dataStruct = global->apoapseData->GetStructure("server_info");

		dataStruct.GetField("username").SetValue(username);

		if (connection.server.usersManager->GetRegisteredUsersCount() > 0)
		{
			if (UsersManager::LoginIsValid(username, data.GetField("password").GetValue<ByteContainer>()))
			{
				connection.Authenticate(username);

				if (connection.GetRelatedUser()->IsUsingTemporaryPassword())
				{
					dataStruct.GetField("status").SetValue("requirePasswordChange");
				}
				else
				{
					dataStruct.GetField("status").SetValue("authenticated");

					{
						auto usergroupsDat = global->apoapseData->ReadListFromDatabase("usergroup", "", "");
						dataStruct.GetField("usergroups").SetValue(usergroupsDat);
					}

					dataStruct.GetField("nickname").SetValue(connection.GetRelatedUser()->GetNickname());
					dataStruct.GetField("usergroup").SetValue(connection.GetRelatedUser()->GetUsergroup().GetUuid());
				}
			}
			else
			{
				SecurityLog::LogAlert(ApoapseErrorCode::unable_to_authenticate_user, connection);
				return;
			}
		}
		else
		{
			LOG << "No users and usergroups registered: in first setup state";

			dataStruct.GetField("status").SetValue("setup_state");
		}

		global->cmdManager->CreateCommand("server_info", dataStruct).Send(connection);
	}

	else if (cmd.name == "install")
	{
		const auto username = cmd.GetData().GetField("admin_username").GetValue<Username>();
		const auto password = cmd.GetData().GetField("admin_password").GetValue<ByteContainer>();

		if (connection.server.usersManager->GetRegisteredUsersCount() == 0)
		{
			const Uuid adminUsergroup = Uuid::Generate();

			connection.server.usergroupManager->CreateUsergroup(adminUsergroup, "@admin", "CREATE_USER CREATE_USERGROUP");
			connection.server.usergroupManager->CreateUsergroup(Uuid::Generate(), "@user", "");

			connection.server.usersManager->RegisterNewUser(username, password, adminUsergroup);
			connection.server.usersManager->SetUserIdentity(username, password, cmd.GetData().GetField("admin_nickname").GetValue<std::string>());

			LOG << "Apoapse setup complete. Disconnecting administrator for first connection.";
			connection.Close();
		}
		else
		{
			LOG << LogSeverity::error << "Trying to use the apoapse_install cmd while the server is not in setup phase.";
			connection.Close();
		}
	}

	else if (cmd.name == "request_sync")
	{
		ApoapseOperation::ExecuteSyncRequest(data.GetField("last_op_time").GetValue<Int64>(), netConnection);
	}

	else if (cmd.name == "add_user")
	{
		const Username username = cmd.GetData().GetField("username").GetValue<Username>();
		const auto tempPassword = cmd.GetData().GetField("temp_password").GetValue<ByteContainer>();
		const Uuid usergroup = cmd.GetData().GetField("usergroup").GetValue<Uuid>();

		if (!connection.server.usersManager->DoesUserExist(username))
		{
			connection.server.usersManager->RegisterNewUser(username, tempPassword, usergroup);
		}
		else
		{
			SecurityLog::LogAlert(ApoapseErrorCode::unable_to_register_user, connection);
		}
	}

	else if (cmd.name == "set_identity")
	{
		const ByteContainer encryptedPassword = cmd.GetData().GetField("password").GetValue<ByteContainer>();
		const std::string nickname = cmd.GetData().GetField("nickname").GetValue<std::string>();

		connection.server.usersManager->SetUserIdentity(connection.GetRelatedUser()->GetUsername(), encryptedPassword, nickname);

		LOG << "First connection: set identity complete. Disconnecting the user for first connection with the actual password.";
		connection.Close();
	}
}

void ServerCmdManager::OnReceivedCommandPost(CommandV2& cmd, GenericConnection& netConnection)
{
	auto& connection = dynamic_cast<ServerConnection&>(netConnection);

	if (cmd.name == "create_room")
	{
		// We create the default thread if the room layout is of type "single" 
		if (cmd.GetData().GetField("threads_layout").GetValue<std::string>() == "single")
		{
			auto dat = global->apoapseData->GetStructure("thread");
			dat.GetField("uuid").SetValue(Uuid::Generate());
			dat.GetField("parent_room").SetValue(cmd.GetData().GetField("uuid").GetValue<Uuid>());
			dat.GetField("name").SetValue("default");

			dat.SaveToDatabase();

			auto cmdThread = global->cmdManager->CreateCommand("create_thread", dat);
			ApoapseOperation::RegisterOperation(cmdThread, connection.GetConnectedUser());
			Propagate(cmdThread, connection);
		}
	}
}

void ServerCmdManager::Propagate(CommandV2& cmd, GenericConnection& netConnection)
{
	//TODO Complete server cmd propagation with the read_permission field taken into consideration from the data structure json
	auto& connection = dynamic_cast<ServerConnection&>(netConnection);

	GenericConnection* propagateToSelf = (cmd.excludeSelfPropagation) ? &connection : nullptr;
	cmd.Send(*connection.server.usersManager, propagateToSelf);	//We send to all the connected users
}
