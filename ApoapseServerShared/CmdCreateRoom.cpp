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

class CmdCreateRoom final : public Command
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

		{
			SQLQuery query(*global->database);
			query << INSERT_INTO << "rooms" << " (uuid)" << VALUES << "(" << uuid.GetInRawFormat() << ")";

			query.Exec();
		}
		
		Operation(OperationType::new_room, OperationDirection::sent, sender.GetUsername(), uuid).Save();

		/*{
			MessagePackSerializer ser;
			ser.UnorderedAppend("uuid", uuid.GetInRawFormat());

			//Propagate(ser, *senderConnection.server.usersManager);

			CmdCreateRoom cmd;
			cmd.Send(ser, *senderConnection.server.usersManager);
		}*/

		Propagate(*senderConnection.server.usersManager);
	}
};

APOAPSE_COMMAND_REGISTER(CmdCreateRoom, CommandId::create_room);