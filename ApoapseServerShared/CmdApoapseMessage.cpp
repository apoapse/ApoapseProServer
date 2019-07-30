#include "stdafx.h"
#include "Common.h"
#include "Command.h"
#include "CommandsManager.h"
#include "ServerConnection.h"
#include "ApoapseServer.h"
#include "SecurityAlert.h"
#include "SQLQuery.h"
#include "UsersManager.h"
#include "DateTimeUtils.h"
#include "SQLUtils.hpp"

class CmdApoapseMessage final : public Command
{
public:
	CommandInfo& CmdApoapseMessage::GetInfo() const override
	{
		static auto info = CommandInfo();
		info.command = CommandId::apoapse_message;
		info.requireAuthentication = true;
		info.fields =
		{
			Field{ "uuid", FieldRequirement::any_mendatory, FIELD_VALUE_VALIDATOR(std::vector<byte>, Uuid::IsValid) },
			Field{ "threadUuid", FieldRequirement::any_mendatory, FIELD_VALUE_VALIDATOR(std::vector<byte>, Uuid::IsValid) },
			Field{ "sentTime", FieldRequirement::any_mendatory, FIELD_VALUE_VALIDATOR(std::string, DateTimeUtils::UTCDateTime::ValidateFormat) },
			Field{ "content", FieldRequirement::any_mendatory, FIELD_VALUE_VALIDATOR(std::vector<byte>, [&](const auto& data) { return !data.empty();}) },
		};

		return info;
	}

	void Process(User& sender, ServerConnection& senderConnection) override
	{
		const auto sentTime = DateTimeUtils::UTCDateTime(GetFieldsData().GetValue<std::string>("sentTime"));
		const auto uuid = Uuid(GetFieldsData().GetValue<ByteContainer>("uuid"));
		const DbId dbId = SQLUtils::CountRows("messages");

		if (sentTime > DateTimeUtils::UTCDateTime::CurrentTime())
		{
			SecurityLog::LogAlert(ApoapseErrorCode::future_datetime, senderConnection, &uuid);
			return;
		}

		{
			SQLQuery query(*global->database);
			query	<< INSERT_INTO << "messages" << " (id, uuid, thread_uuid, author, sent_time, content)" << VALUES << "(" 
					<< dbId << "," << uuid.GetBytes() << "," << GetFieldsData().GetValue<ByteContainer>("threadUuid") << ","
					<< sender.GetUsername().GetRaw() << "," << sentTime.GetStr() << "," << GetFieldsData().GetValue<ByteContainer>("content")
			<< ")";

			query.Exec();
		}

		LOG << "Received message " << uuid.GetBytes() << " from " << sender.GetUsername().ToStr();

		//Operation(OperationType::new_message, sender.GetUsername(), dbId).Save();

		{
			MessagePackSerializer ser;
			ser.UnorderedAppend("uuid", uuid.GetInRawFormat());
			ser.UnorderedAppend("threadUuid", GetFieldsData().GetValue<ByteContainer>("threadUuid"));
			ser.UnorderedAppend("sentTime", sentTime.GetStr());
			ser.UnorderedAppend("author", sender.GetUsername().GetRaw());
			ser.UnorderedAppend("content", GetFieldsData().GetValue<ByteContainer>("content"));

			Propagate(ser, *senderConnection.server.usersManager);
		}
	}
};

APOAPSE_COMMAND_REGISTER(CmdApoapseMessage, CommandId::apoapse_message);