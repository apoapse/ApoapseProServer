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

class CmdCreateThread final : public Command
{
public:
	CommandInfo& CmdCreateThread::GetInfo() const override
	{
		static auto info = CommandInfo();
		info.command = CommandId::create_thread;
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
		const DbId dbid = SQLUtils::CountRows("threads");

		{
			SQLQuery query(*global->database);
			query << SELECT << "id" << FROM << "threads" << WHERE << "uuid" << EQUALS << uuid.GetInRawFormat();

			auto res = query.Exec();
			int count = res.RowCount();
			int ts = 0;
		}

		{
			SQLQuery query(*global->database);
			query << INSERT_INTO << "threads" << " (id, uuid)" << VALUES << "(" << dbid << "," << uuid.GetInRawFormat() << ")";

			query.Exec();
		}

		Operation(OperationType::new_thread, OperationDirection::sent, sender.GetUsername(), uuid).Save();

		Propagate(*senderConnection.server.usersManager);
	}
};

APOAPSE_COMMAND_REGISTER(CmdCreateThread, CommandId::create_thread);