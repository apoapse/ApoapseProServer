#include "stdafx.h"
#include "Common.h"
#include "CommandsManager.h"
#include "User.h"
#include "UsersManager.h"
#include "ApoapseServer.h"
#include "ServerConnection.h"
#include "SecurityAlert.h"

class CmdMarkMessageAsRead final : public Command
{
public:
	CommandInfo& GetInfo() const override
	{
		static auto info = CommandInfo();
		info.command = CommandId::mark_message_as_read;
		info.requireAuthentication = true;
		info.fields =
		{
			Field{ "messageUuid", FieldRequirement::any_mendatory, FIELD_VALUE_VALIDATOR(std::vector<byte>, Uuid::IsValid) },
		};

		return info;
	}

	void Process(User& sender, ServerConnection& senderConnection) override
	{
		const auto msgUuid = Uuid(GetFieldsData().GetValue<ByteContainer>("messageUuid"));

		/*if (ApoapseMessage::DoesMessageExist(msgUuid))
		{
			Operation(OperationType::mark_message_as_read, sender.GetUsername(), ApoapseMessage::GetMessageDbIdByUuid(msgUuid), OperationOwnership::sender_user).Save();

			Propagate(senderConnection);
		}
		else
		{
			SecurityLog::LogAlert(ApoapseErrorCode::related_item_does_not_exist, sender);
		}*/
	}

};

APOAPSE_COMMAND_REGISTER(CmdMarkMessageAsRead, CommandId::mark_message_as_read);
