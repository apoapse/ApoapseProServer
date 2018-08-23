#include "stdafx.h"
#include "Common.h"
#include "CommandsManager.h"
#include "User.h"
#include "UsersManager.h"
#include "ApoapseServer.h"
#include "ServerConnection.h"
#include "OperationObjects.h"

class CmdSyncRequest final : public Command
{
public:
	CommandInfo& GetInfo() const override
	{
		static auto info = CommandInfo();
		info.command = CommandId::sync_request;
		info.serverOnly = true;
		info.requireAuthentication = true;
		info.fields =
		{
			Field{ "sinceTimestamp", FieldRequirement::any_mendatory, FIELD_VALUE(Int64) },
		};

		return info;
	}

	void Process(User& sender, ServerConnection& senderConnection) override
	{
		Int64 sinceTimestamp = GetFieldsData().GetValue<Int64>("sinceTimestamp");

		OperationObjects::SynchronizeSince(sinceTimestamp, senderConnection);
	}
};

APOAPSE_COMMAND_REGISTER(CmdSyncRequest, CommandId::sync_request);