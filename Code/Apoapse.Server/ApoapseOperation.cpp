#include "stdafx.h"
#include "ApoapseOperation.h"
#include "Common.h"
#include "ApoapseServer.h"
#include "LocalUser.h"
#include "Uuid.h"
#include "GenericConnection.h"
#include "Command.h"
#include "RemoteServersManager.h"
#include "RemoteServer.h"

ApoapseOperation::ApoapseOperation(const Uuid& uuid, const string& name, const string& itemDbTableName, OperationDirection dir, ApoapseServer& serverRef)
	: m_uuid(uuid),
	m_name(name),
	m_itemDBTableName(itemDbTableName),
	m_direction(dir),
	server(serverRef)
{
}

OperationDirection ApoapseOperation::GetDirection() const
{
	return m_direction;
}

void ApoapseOperation::SaveToDatabase(GenericConnection& originConnection)
{
	if (IsIemRegistered())
	{
		LOG << "The item uuid is already registered in the database table " << m_itemDBTableName << " (" << m_uuid.GetStr() << ")" << LogSeverity::warning;
		ApoapseError::SendError(ApoapseErrorCode::ITEM_UUID_ALREADY_REGISTERED, m_uuid.GetStr(), originConnection);

		return;
	}

	SaveToDatabaseInternal();

	{
		// Get the id of the newly created database entry for the saved item
		SQLQuery queryId(server.database);
		queryId << SELECT << "id" << FROM << STR_TO_QUERY_SYNTAX(m_itemDBTableName) << " " << WHERE << "uuid" << EQUALS << m_uuid.GetStr();
		auto res = queryId.Exec();

		if (res.RowCount() != 1)
			throw std::exception();

		SetItemDbId(res[0][0].GetInt64());
	}

	if (m_itemId.get() < 1)
		throw std::out_of_range("Invalid item database id");}

void ApoapseOperation::SetItemDbId(DbId id)
{
	ASSERT_MSG(!m_itemId.is_initialized(), "ItemDbId already definied");

	m_itemId = id;
}

DbId ApoapseOperation::GetItemDbId() const
{
	ASSERT(m_itemId.is_initialized());

	return m_itemId.get();
}

void ApoapseOperation::Send(const std::vector<ApoapseAddress>& recipients, GenericConnection& originConnection)
{
	auto cmd = PrepareCommandToBeSent();

	for (auto& address : recipients)
	{
		// Recipient on the current server
		if (address.GetDomain() == UsersManager::GetCurrentServerDomain())
		{
			if (server.GetUsersManager().IsConnected(address.GetUsernameHash()))
			{
				// The recipient is currently connected
				auto user = server.GetUsersManager().GetConnectedUser(address.GetUsernameHash());

				LogOperation(user.GetDatabaseId(), OperationDirection::RECEIVED);
				cmd->Send(user);
			}
			else
			{
				// The recipient is not connected
				if (DbId userDbId = server.GetUsersManager().TryGetUser(address.GetUsernameHash()))
				{
					LogOperation(userDbId, OperationDirection::RECEIVED);
				}
				else
				{
					ApoapseError::SendError(ApoapseErrorCode::RECIPIENT_DOES_NOT_EXIST, address.GetFullAddress(), originConnection);
					continue;
				}
			}
		}
		else
		{
			// Recipient is on a remote server
			if (RemoteServer* connectedServer = server.GetRemoteServersManager().TryGetRemoteServer(address.GetDomain()))
			{
				// The remote server is connected
				connectedServer->EnqueCommandToBeSend(std::move(cmd));
			}
			else
			{
				// A connection to a remote server need to be created
				std::shared_ptr<RemoteServer> remoteServer = server.GetRemoteServersManager().CreateConnection(server);
				remoteServer->EnqueCommandToBeSend(std::move(cmd));
				remoteServer->ConnectToRemoteServer(address.GetDomain());
			}
		}
	}
}

Uuid ApoapseOperation::GetItemUuid() const
{
	return m_uuid;
}

bool ApoapseOperation::IsIemRegistered()
{
	SQLQuery query(server.database);
	query << SELECT << "uuid" << FROM << STR_TO_QUERY_SYNTAX(m_itemDBTableName) << WHERE << "uuid" EQUALS << m_uuid.GetStr();
	auto res = query.Exec();

	return (res.RowCount() == 1);
}

const ApoapseOperation::DBInfo ApoapseOperation::GetOperationInfoFromDatabase(DbId itemId, const string& operationName, ApoapseServer& server)
{
	SQLQuery query(server.database);
	query << SELECT << "id,timestamp,direction,user_id" << FROM << "operations_log" << WHERE << "operation" << EQUALS << operationName << AND << "item_id" << EQUALS << itemId;
	auto res = query.Exec();

	if (res.RowCount() == 0)
		throw std::out_of_range("The requested item id does not exist on the database");

	DBInfo output;
	output.operationId = res[0][0].GetInt64();
	output.timestamp = res[0][1].GetText();
	output.direction = (res[0][2].GetText() == "R") ? OperationDirection::RECEIVED : OperationDirection::SENT;
	output.associatedUserId = res[0][3].GetInt64();

	return output;
}

void ApoapseOperation::LogOperation(DbId associatedUserId, OperationDirection direction /*= OperationDirection::SENT*/)
{
	ASSERT_MSG(m_itemId.is_initialized(), "ApoapseOperation::SaveToDatabase might not have been called");

	string dirStr = (direction == OperationDirection::RECEIVED) ? "R" : "S";

	SQLQuery query(server.database);
	query << INSERT_INTO << "operations_log (operation, direction, item_id, user_id)" << VALUES << "(" << m_name << "," << dirStr << "," << m_itemId.get() << "," << associatedUserId << ")";
	//query << INSERT_INTO << "operations_log (timestamp, operation, direction, item_id, user_id)" << VALUES << "(" << TIMESTAMP << "," << m_name << "," << dir << "," << m_itemId.get() << "," << associatedUserId << ")";
	query.ExecAsync();

	LOG << "Logged new operation on the database for item " << m_name << ":" << m_uuid << " DbId: " << m_itemId.get() << LogSeverity::verbose;
}
