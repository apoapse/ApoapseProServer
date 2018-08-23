#include "stdafx.h"
#include "Common.h"
#include "CommandsManager.h"
#include "User.h"
#include "UsersManager.h"
#include "ApoapseServer.h"
#include "ServerConnection.h"

class CmdApoapseInstall final : public Command
{
public:
	CommandInfo& GetInfo() const override
	{
		static auto info = CommandInfo();
		info.command = CommandId::apoapse_install;
		info.serverOnly = true;
		info.onlyNonAuthenticated = true;
		info.fields =
		{
			Field{ "admin_username", FieldRequirement::any_mendatory, FIELD_VALUE_VALIDATOR(std::vector<byte>, Username::IsValid) },
			Field{ "admin_password", FieldRequirement::any_mendatory, FIELD_VALUE_VALIDATOR(std::vector<byte>, [&](const auto& hash) { return (hash.size() == sha256Length); }) },
		};

		return info;
	}

	void Process(ServerConnection& sender) override
	{
		const auto username = Username(GetFieldsData().GetValue<std::vector<byte>>("admin_username"));
		const auto password = GetFieldsData().GetValue<std::vector<byte>>("admin_password");

		if (sender.server.usersManager->GetRegisteredUsersCount() == 0)
		{
			sender.server.usersManager->RegisterNewUser(username, password);
			sender.server.usersManager->SetUserIdentity(username, password);

			LOG << "Apoapse setup complete. Disconnecting administrator for first connection.";	// #TODO authenticate directly the user
			sender.Close();
		}
		else
		{
			LOG << LogSeverity::error << "Trying to use the apoapse_install cmd while the server is not in setup phase.";
			sender.Close();
		}
	}
};

APOAPSE_COMMAND_REGISTER(CmdApoapseInstall, CommandId::apoapse_install);