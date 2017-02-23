#include "stdafx.h"
#include "Common.h"
#include "CommandsManager.h"
#include "ClientConnection.h"
#include "LocalUser.h"
#include "Uuid.h"
#include "DateTimeUtils.h"
#include "Message.h"
#include "Conversation.h"

class CmdMessage final : public Command
{
public:
	const CommandConfig& GetConfig() override
	{
		static auto config = CommandConfig();
		config.name = "MESSAGE";
		config.expectedFormat = Format::JSON;
		config.payloadExpected = true;
		config.processFromUser = PROCESS_METHOD_FROM_USER(CmdMessage::FromUser);
		//config.processFromRemoteServer = PROCESS_METHOD(RemoteServer&, )
		config.fields =
		{
			CommandField{ "uuid", FieldRequirement::VALUE_MENDATORY, FIELD_VALUE_VALIDATOR(string, Uuid::IsValid) },
			CommandField{ "from", FieldRequirement::ANY_OPTIONAL, FIELD_VALUE_VALIDATOR(string, [](string address) { return ApoapseAddress(address).IsValid(); }) },
			CommandField{ "conversation", FieldRequirement::VALUE_MENDATORY, FIELD_VALUE_VALIDATOR(string, Uuid::IsValid) },
			CommandField{ "sent", FieldRequirement::VALUE_MENDATORY, FIELD_VALUE_VALIDATOR(string, DateTimeUtils::UTCDateTime::ValidateFormat) }
		};

		return config;
	}

	bool PostValidate() const override
	{
		return true;
	}

private:
	void FromUser(LocalUser& user, ClientConnection& originConnection)
	{
		const auto uuid = Uuid(ReadFieldValue<string>("uuid").get());
		const auto conversationUuid = Uuid(ReadFieldValue<string>("conversation").get());
		const auto sentTime = DateTimeUtils::UTCDateTime(ReadFieldValue<string>("sent").get());

		if (sentTime > DateTimeUtils::UTCDateTime::CurrentTime())
		{
			ApoapseError::SendError(ApoapseErrorCode::DATETIME_MISMATCH, uuid, originConnection);
			return;
		}

		try
		{
			Conversation conv = Conversation::Create(conversationUuid, originConnection.server);

			Message message(uuid, user.GetFullAddress(), conv, sentTime, GetPayload(), OperationDirection::SENT, originConnection.server);
			//message.Send(user, originConnection);
			message.SaveToDatabase(user, originConnection);
		}
		catch (const std::out_of_range&)
		{
			ApoapseError::SendError(ApoapseErrorCode::CONVERSATION_NONEEXISTENT, uuid, originConnection);
		}
	}
};

APOAPSE_COMMAND_REGISTER(CmdMessage, "MESSAGE");