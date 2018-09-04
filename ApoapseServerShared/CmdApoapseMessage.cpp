#include "stdafx.h"
#include "Common.h"
#include "Command.h"
#include "CommandsManager.h"
#include "ServerConnection.h"
#include "ApoapseServer.h"
#include "SecurityAlert.h"
#include "Operation.h"
#include "SQLQuery.h"
#include "UsersManager.h"
#include "DateTimeUtils.h"
#include "SQLUtils.hpp"
#include "OperationObjects.h"

class CmdApoapseMessage final : public Command, public IOperationObject
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
					<< dbId << "," << uuid.GetAsByteVector() << "," << GetFieldsData().GetValue<ByteContainer>("threadUuid") << ","
					<< sender.GetUsername().GetRaw() << "," << sentTime.str() << "," << GetFieldsData().GetValue<ByteContainer>("content")
			<< ")";

			query.Exec();
		}

		LOG << "Received message " << uuid.GetAsByteVector() << " from " << sender.GetUsername().ToStr();

		Operation(OperationType::new_message, sender.GetUsername(), dbId).Save();

		{
			MessagePackSerializer ser;
			ser.UnorderedAppend("uuid", uuid.GetInRawFormat());
			ser.UnorderedAppend("threadUuid", GetFieldsData().GetValue<ByteContainer>("threadUuid"));
			ser.UnorderedAppend("sentTime", sentTime.str());
			ser.UnorderedAppend("author", sender.GetUsername().GetRaw());
			ser.UnorderedAppend("content", GetFieldsData().GetValue<ByteContainer>("content"));

			Propagate(ser, *senderConnection.server.usersManager);
		}
	}

	void SendFromDatabase(DbId id, INetworkSender& connection) override
	{
		SQLQuery query(*global->database);
		query << SELECT << "uuid,thread_uuid,author,sent_time,content" << FROM << "messages" << WHERE << "id" << EQUALS << id;
		auto res = query.Exec();

		MessagePackSerializer ser;
		ser.UnorderedAppend("uuid", res[0][0].GetByteArray());
		ser.UnorderedAppend("threadUuid", res[0][1].GetByteArray());
		ser.UnorderedAppend("author", res[0][2].GetByteArray());
		ser.UnorderedAppend("sentTime", res[0][3].GetText());
		ser.UnorderedAppend("content", res[0][4].GetByteArray());

		CmdApoapseMessage cmd;
		cmd.Send(ser, connection);
	}
};

APOAPSE_COMMAND_REGISTER(CmdApoapseMessage, CommandId::apoapse_message);
REGISTER_OPERATION_OBJECT(CmdApoapseMessage, OperationType::new_message);