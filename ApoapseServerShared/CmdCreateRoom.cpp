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
#include "OperationObjects.h"
#include "SQLUtils.hpp"
#include "ApoapseMetadata.h"

class CmdCreateRoom final : public Command, public IOperationObject
{
public:
	CommandInfo& CmdCreateRoom::GetInfo() const override
	{
		static auto info = CommandInfo();
		info.command = CommandId::create_room;
		info.serverOnly = true;
		info.requireAuthentication = true;
		info.metadataTypes = MetadataAcess::self;
		info.fields =
		{
			Field{ "uuid", FieldRequirement::any_mendatory, FIELD_VALUE_VALIDATOR(std::vector<byte>, Uuid::IsValid) }
		};

		return info;
	}

	void Process(User& sender, ServerConnection& senderConnection) override
	{
		const auto uuid = Uuid(GetFieldsData().GetValue<ByteContainer>("uuid"));
		const DbId dbId = SQLUtils::CountRows("rooms");

		{
			SQLQuery query(*global->database);
			query << INSERT_INTO << "rooms" << " (id, uuid, metadata_self)" << VALUES << "(" << dbId << "," << uuid.GetInRawFormat() << "," << GetMetadataField(MetadataAcess::self).GetRawDataForDb() << ")";

			query.Exec();
		}
		
		Operation(OperationType::new_room, sender.GetUsername(), dbId).Save();

		Propagate(*senderConnection.server.usersManager);
	}

	void SendFromDatabase(DbId id, INetworkSender& connection) override
	{
		SQLQuery query(*global->database);
		query << SELECT << ALL << FROM << "rooms" << WHERE << "id" << EQUALS << id;
		auto res = query.Exec();

		MessagePackSerializer ser;
		ser.UnorderedAppend("uuid", res[0][1].GetByteArray());
		ser.UnorderedAppend("metadata_self", ApoapseMetadata(res[0][2], MetadataAcess::self).GetRawData());

		CmdCreateRoom cmd;
		cmd.Send(ser, connection);
	}
};

APOAPSE_COMMAND_REGISTER(CmdCreateRoom, CommandId::create_room);
REGISTER_OPERATION_OBJECT(CmdCreateRoom, OperationType::new_room);