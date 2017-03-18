#include "stdafx.h"
#include "Common.h"
#include "CommandsManager.h"
#include "ClientConnection.h"
#include "LocalUser.h"
#include "Uuid.h"
#include "DateTimeUtils.h"
#include "Message.h"
#include "Conversation.h"
#include "Command.h"
#include "RemoteServer.h"

class CmdMessage final : public Command
{
public:
	const CommandConfig& GetConfig() override
	{
		static auto config = CommandConfig();
		config.name = "MESSAGE";
		config.propagateToUser = true;
		config.expectedFormat = Format::JSON;
		config.payloadExpected = true;
		config.processFromUser = PROCESS_METHOD_FROM_USER(CmdMessage::FromUser);
		config.processFromRemoteServer = PROCESS_METHOD(RemoteServer, CmdMessage::FromExternal);
		config.fields =
		{
			CommandField{ "uuid", FieldRequirement::VALUE_MENDATORY, FIELD_VALUE_VALIDATOR(string, Uuid::IsValid) },
			CommandField{ "from", FieldRequirement::ANY_OPTIONAL, FIELD_VALUE_VALIDATOR(string, [](string address) { return ApoapseAddress(address).IsValid(); }) },
			CommandField{ "conversation", FieldRequirement::VALUE_MENDATORY, FIELD_VALUE_VALIDATOR(string, Uuid::IsValid) },
			CommandField{ "sent", FieldRequirement::VALUE_MENDATORY, FIELD_VALUE_VALIDATOR(string, DateTimeUtils::UTCDateTime::ValidateFormat) }
		};

		return config;
	}

	bool PostValidate() override
	{
		return true;
	}

private:
	void FromExternal(RemoteServer& remoteServer)
	{
		ReceiveMessage(remoteServer, ApoapseAddress(ReadFieldValue<string>("sent").get()));
	}

	bool FromUser(LocalUser& user, ClientConnection& originConnection)
	{
		return ReceiveMessage(originConnection, user.GetFullAddress(), user.GetDatabaseId());
	}

	bool ReceiveMessage(GenericConnection& originConnection, const ApoapseAddress& sender, DbId associatedUser = 0)
	{
		const auto uuid = Uuid(ReadFieldValue<string>("uuid").get());
		const auto conversationUuid = Uuid(ReadFieldValue<string>("conversation").get());
		const auto sentTime = DateTimeUtils::UTCDateTime(ReadFieldValue<string>("sent").get());

		if (sentTime > DateTimeUtils::UTCDateTime::CurrentTime())
		{
			ApoapseError::SendError(ApoapseErrorCode::DATETIME_MISMATCH, uuid, originConnection);
			return false;
		}

		try
		{
			auto conv = Conversation::Create(conversationUuid, originConnection.server);

			Message message(uuid, sender, conv, sentTime, GetPayloadPtr(), OperationDirection::SENT, originConnection.server);
			message.Send(message.GetCorrespondents(), originConnection);
			message.SaveToDatabase(originConnection);

			if (associatedUser)
				message.LogOperation(associatedUser);

			return true;
		}
		catch (const std::out_of_range&)
		{
			ApoapseError::SendError(ApoapseErrorCode::CONVERSATION_NONEEXISTENT, uuid, originConnection);
		}

		return false;
	}
};

APOAPSE_COMMAND_REGISTER(CmdMessage, "MESSAGE");