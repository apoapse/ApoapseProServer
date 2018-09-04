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
#include "SQLUtils.hpp"
#include "OperationObjects.h"

class CmdCreateThread final : public Command, public IOperationObject
{
public:
	CommandInfo& CmdCreateThread::GetInfo() const override
	{
		static auto info = CommandInfo();
		info.command = CommandId::create_thread;
		info.serverOnly = true;
		info.requireAuthentication = true;
		info.metadataTypes = MetadataAcess::usergroup;
		info.fields =
		{
			Field{ "uuid", FieldRequirement::any_mendatory, FIELD_VALUE_VALIDATOR(ByteContainer, Uuid::IsValid) },
			Field{ "room_uuid", FieldRequirement::any_mendatory, FIELD_VALUE_VALIDATOR(ByteContainer, Uuid::IsValid) },
		};

		return info;
	}

	void Process(User& sender, ServerConnection& senderConnection) override
	{
		const auto uuid = Uuid(GetFieldsData().GetValue<ByteContainer>("uuid"));
		const auto roomUuid = Uuid(GetFieldsData().GetValue<ByteContainer>("room_uuid"));
		const DbId dbid = SQLUtils::CountRows("threads");

		{
			SQLQuery query(*global->database);
			query << INSERT_INTO << "threads" << " (id, uuid, room_uuid, metadata_usergroup)" << VALUES << "(" << dbid << "," << uuid.GetInRawFormat() << "," << roomUuid.GetInRawFormat() << "," << GetMetadataField(MetadataAcess::usergroup).GetRawDataForDb() << ")";

			query.Exec();
		}

		Operation(OperationType::new_thread, sender.GetUsername(), dbid).Save();

		Propagate(*senderConnection.server.usersManager);
	}

	void SendFromDatabase(DbId id, INetworkSender& connection) override
	{
		SQLQuery query(*global->database);
		query << SELECT << "room_uuid,uuid,metadata_usergroup" << FROM << "threads" << WHERE << "id" << EQUALS << id;
		auto res = query.Exec();

		MessagePackSerializer ser;
		ser.UnorderedAppend("uuid", res[0][1].GetByteArray());
		ser.UnorderedAppend("room_uuid", res[0][0].GetByteArray());
		ser.UnorderedAppend("metadata_usergroup", ApoapseMetadata(res[0][2], MetadataAcess::usergroup).GetRawData());

		CmdCreateThread cmd;
		cmd.Send(ser, connection);
	}
};

APOAPSE_COMMAND_REGISTER(CmdCreateThread, CommandId::create_thread);
REGISTER_OPERATION_OBJECT(CmdCreateThread, OperationType::new_thread);