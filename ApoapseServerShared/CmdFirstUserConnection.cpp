#include "stdafx.h"
#include "Common.h"
#include "CommandsManager.h"
#include "User.h"
#include "UsersManager.h"
#include "ApoapseServer.h"
#include "ServerConnection.h"

class CmdFirstUserConnection final : public Command
{
public:
	CommandInfo& GetInfo() const override
	{
		static auto info = CommandInfo();
		info.command = CommandId::first_user_connection;
		info.serverOnly = true;
		info.allowForUsersRequiredToChangePassword = true;
		info.fields =
		{
			Field{ "password", FieldRequirement::any_mendatory, FIELD_VALUE_VALIDATOR(std::vector<byte>, [&](const auto& hash) { return (hash.size() == sha256Length); }) },
		};

		info.metadataTypes = MetadataAcess::all;

		return info;
	}

	void Process(User& sender, ServerConnection& senderConnection) override
	{
		if (sender.IsUsingTemporaryPassword())
		{
			const auto password = GetFieldsData().GetValue<std::vector<byte>>("password");
			senderConnection.server.usersManager->SetUserIdentity(sender.GetUsername(), password, GetMetadataField(MetadataAcess::all));

			// propagate
			{
				//OperationObjects::CreateObject(OperationType::new_user)->SendFromDatabase()
			}

			LOG << "First connection setup complete. Disconnecting the user for first connection with the actual password.";	// #TODO authenticate directly the user
			senderConnection.Close();
		}
		else
		{
			LOG << LogSeverity::error << "Trying to use the first_user_connection command to an user ";
		}
	}
};

APOAPSE_COMMAND_REGISTER(CmdFirstUserConnection, CommandId::first_user_connection);