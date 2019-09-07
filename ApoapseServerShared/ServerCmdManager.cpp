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
#include "ServerFileStreamConnection.h"
#include "ByteUtils.hpp"
#include <filesystem>

ServerCmdManager::ServerCmdManager() : CommandsManagerV2(GetCommandDef())
{
	
}

bool ServerCmdManager::OnSendCommandPre(CommandV2& cmd)
{
	if (cmd.name == "new_message")
	{
		const Uuid msgUuid = cmd.GetData().GetField("uuid").GetValue<Uuid>();

		if (!cmd.GetData().GetField("attachments").HasValue())
		{
			auto dbDat = global->apoapseData->ReadListFromDatabase("attachment", "parent_message", msgUuid);
			cmd.GetData().GetField("attachments").SetValue(dbDat);
		}
	}
	
	return true;
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

		// Write server info
		auto dataStruct = global->apoapseData->GetStructure("server_info");

		dataStruct.GetField("username").SetValue(username);

		if (connection.server.usersManager->GetRegisteredUsersCount() > 0)
		{
			if (UsersManager::LoginIsValid(username, data.GetField("password").GetValue<ByteContainer>()))
			{
				User& relatedUser = connection.Authenticate(username);

				if (connection.GetRelatedUser()->IsUsingTemporaryPassword())
				{
					dataStruct.GetField("status").SetValue("requirePasswordChange");
				}
				else
				{
					dataStruct.GetField("status").SetValue("authenticated");

					{
						FileStreamAuth fileStreamAuth = relatedUser.GenerateFileStreamAuthCode(connection);
						dataStruct.GetField("file_stream_auth_code").SetValue(ArrayToVector<byte, sha256Length>(fileStreamAuth.code));
					}

					{
						auto usergroupsDat = global->apoapseData->ReadListFromDatabase("usergroup", "", "");
						dataStruct.GetField("usergroups").SetValue(usergroupsDat);
					}

					{
						std::vector<DataStructure> connectedUsers;

						for (const User* user : connection.server.usersManager->GetConnectedUsers())
						{
							auto connectedUserDat = global->apoapseData->GetStructure("connected_user");
							connectedUserDat.GetField("user").SetValue(user->GetUsername());

							connectedUsers.push_back(connectedUserDat);
						}

						dataStruct.GetField("connected_users").SetValue(connectedUsers);
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

	else if (cmd.name == "attachment_download")
	{
		const Uuid uuid = cmd.GetData().GetField("uuid").GetValue<Uuid>();
		DataStructure dat = global->apoapseData->ReadItemFromDatabase("attachment", "uuid", uuid);

		if (!dat.GetField("is_downloaded").GetValue<bool>())
		{
			LOG << LogSeverity::error << "The client in requesting to download a file that has not be uploaded on the server";
			return;
		}
		
		AttachmentFile file(dat, GenerateAttachmentPath(uuid));
		connection.GetFileStream()->PushFileToSend(file);
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
			ApoapseOperation::RegisterOperation(cmdThread, connection.GetConnectedUser(), true);
			Propagate(cmdThread, connection);
		}
	}

	else if (cmd.name == "direct_message")
	{
		const Username relatedUser = cmd.GetData().GetField("direct_recipient").GetValue<Username>();

		ApoapseOperation::RegisterOperation(cmd, relatedUser, true);
		
		if (connection.server.usersManager->IsUserConnected(relatedUser))
		{
			User& user = *connection.server.usersManager->GetUserByUsername(relatedUser).lock().get();
			PropagateToUser(cmd, user);
		}
	}
	
	if (cmd.name == "new_message" || cmd.name == "direct_message")
	{
		const Uuid msgUuid = cmd.GetData().GetField("uuid").GetValue<Uuid>();
		auto& attachmentsDat = cmd.GetData().GetField("attachments").GetDataArray();

		if (!attachmentsDat.empty())
		{
			for (DataStructure& dat : attachmentsDat)
			{
				dat.GetField("parent_message").SetValue(msgUuid);
				dat.GetField("is_downloaded").SetValue(false);
				dat.SaveToDatabase();

				const Uuid uuid = dat.GetField("uuid").GetValue<Uuid>();
				
				AttachmentFile file(dat, GenerateAttachmentPath(uuid));
				connection.GetFileStream()->PushFileToReceive(file);
			}

			auto dat = global->apoapseData->GetStructure("empty");
			global->cmdManager->CreateCommand("ready_to_receive_file", dat).Send(connection);
		}
	}
}

void ServerCmdManager::Propagate(CommandV2& cmd, GenericConnection& localConnection)
{
	auto& connection = dynamic_cast<ServerConnection&>(localConnection);

	INetworkSender* destination = nullptr;
	{
		if (cmd.operationOwnership == OperationOwnership::all)
			destination = connection.server.usersManager;

		else if (cmd.operationOwnership == OperationOwnership::self)
			destination = &connection;

		else
			FatalError("Unsupported operation ownership type for propagation");
	}

	GenericConnection* propagateToSelf = (cmd.excludeSelfPropagation) ? &connection : nullptr;
	cmd.Send(*destination, propagateToSelf);	//We send to all the connected users
}

void ServerCmdManager::PropagateToUser(CommandV2& cmd, User& user)
{
	cmd.Send(user, nullptr);
}

std::string ServerCmdManager::GenerateAttachmentPath(const Uuid& fileUuid)
{
	const std::string filePath = "server_userfiles/" + BytesToHexString(fileUuid.GetBytes()) + ".dat";
	return filePath;
}
