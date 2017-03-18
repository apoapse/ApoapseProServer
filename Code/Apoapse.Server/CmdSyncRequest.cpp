#include "stdafx.h"
#include "Common.h"
#include "CommandsManager.h"
#include "LocalUser.h"
#include "SQLQuery.h"
#include "ApoapseServer.h"

#include "Message.h"
#include "Conversation.h"

class CmdSyncRequest final : public Command
{
	enum class SyncType
	{
		ALL_ITEMS,
		TIMESTAMP
	};

	SyncType m_syncType;

protected:
	const CommandConfig& GetConfig() override
	{
		static auto config = CommandConfig();
		config.name = "SYNC_REQUEST";
		config.expectedFormat = Format::INLINE;
		config.processFromUser = PROCESS_METHOD_FROM_USER(CmdSyncRequest::SendOperations);
		config.fields =
		{
			CommandField{ "arg1", FieldRequirement::VALUE_MENDATORY },
		};

		return config;
	}

	bool PostValidate() override
	{
		if (ReadFieldValue<string>("arg1").get() == "all")
		{
			m_syncType = SyncType::ALL_ITEMS;
			return true;
		}
		else
		{
			try
			{
				ReadFieldValue<Int64>("arg1").get();
				m_syncType = SyncType::TIMESTAMP;

				return true;
			}
			catch (const std::exception&)
			{
				return false;
			}
		}
	}

private:
	bool SendOperations(LocalUser& user, ClientConnection& originConnection)
	{
		SQLQuery query(originConnection.server.database);

		if (m_syncType == SyncType::ALL_ITEMS)
			query << SELECT << "operation, item_id, direction" << FROM << "operations_log" << WHERE << "user_id" << EQUALS << user.GetDatabaseId() << ORDER_BY << "timestamp" << DESC;
		else
			query << SELECT << "operation, item_id, direction" << FROM << "operations_log" << WHERE << "user_id" << EQUALS << user.GetDatabaseId() <<
			AND << "timestamp" << " > " << ReadFieldValue<Int64>("arg1").get() << ORDER_BY << "timestamp" << DESC;

		if (const SQLPackagedResult res = query.Exec())
		{
			for (const auto& row : res)
			{
				const string operationName = row[0].GetText();
				const DbId itemId = row[1].GetInt64();
				OperationDirection dir = ApoapseOperation::ReadDirectionFromDatabase(row[2].GetText());

				if (operationName == Conversation::databaseOperationName)
				{
					auto conv = Conversation::Create(itemId, originConnection.server, dir);
					auto cmd = conv->PrepareCommandToBeSent();
					cmd->Send(originConnection);
				}
				else if (operationName == Message::databaseOperationName)
				{
					auto message = Message::Create(itemId, originConnection.server, dir);
					auto cmd = message.PrepareCommandToBeSent();
					cmd->Send(originConnection);
				}
			}

			return true;
		}
		else
			return false;
	}
};

APOAPSE_COMMAND_REGISTER(CmdSyncRequest, "SYNC_REQUEST");