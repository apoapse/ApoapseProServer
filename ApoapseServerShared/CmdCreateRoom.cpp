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

class CmdCreateRoom final : public Command, public IOperationObject
{
public:
	CommandInfo& CmdCreateRoom::GetInfo() const override
	{
		static auto info = CommandInfo();
		info.command = CommandId::create_room;
		info.serverOnly = true;
		info.requireAuthentication = true;
		info.fields =
		{
			CommandField{ "uuid", FieldRequirement::any_mendatory, FIELD_VALUE_VALIDATOR(std::vector<byte>, Uuid::IsValid) },
		};

		return info;
	}

	void Process(User& sender, ServerConnection& senderConnection) override
	{
		const auto uuid = Uuid(GetFieldsData().GetValue<ByteContainer>("uuid"));
		const DbId dbId = SQLUtils::CountRows("rooms");

		{
			SQLQuery query(*global->database);
			query << INSERT_INTO << "rooms" << " (id, uuid)" << VALUES << "(" << dbId << "," << uuid.GetInRawFormat() << ")";

			query.Exec();
		}
		
		Operation(OperationType::new_room, sender.GetUsername(), dbId).Save();

		Propagate(*senderConnection.server.usersManager);
	}

	void SendFromDatabase(DbId id, ServerConnection& connection) override
	{
		SQLQuery query(*global->database);
		query << SELECT << ALL << FROM << "rooms" << WHERE << "id" << EQUALS << id;
		auto res = query.Exec();

		MessagePackSerializer ser;
		ser.UnorderedAppend("uuid", res[0][1].GetByteArray());

		CmdCreateRoom cmd;
		cmd.Send(ser, connection);
	}
};

APOAPSE_COMMAND_REGISTER(CmdCreateRoom, CommandId::create_room);
REGISTER_OPERATION_OBJECT(CmdCreateRoom, OperationType::new_room);