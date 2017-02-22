#include "stdafx.h"
#include "ApoapseOperation.h"
#include "Common.h"
#include "ApoapseServer.h"
#include "LocalUser.h"
#include "Uuid.h"
#include "GenericConnection.h"

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

void ApoapseOperation::SaveToDatabase(const LocalUser& associatedUser, GenericConnection& originConnection)
{
	if (IsIemRegistered())
	{
		LOG << "The item uuid is already registered in the database table " << m_itemDBTableName << " (" << m_uuid.GetStr() << ")" << LogSeverity::warning;
		ApoapseError::SendError(ApoapseErrorCode::ITEM_UUID_ALREADY_REGISTERED, m_uuid.GetStr(), originConnection);

		return;
	}

	m_itemId = SaveToDatabaseInternal();

	if (m_itemId.get() < 1)
		throw std::out_of_range("Invalid database id");

	LogOperation(associatedUser.GetDatabaseId());
}

bool ApoapseOperation::IsIemRegistered()
{
	SQLQuery query(server.database);
	query << SELECT << "uuid" << FROM << m_itemDBTableName.c_str() << WHERE << "uuid" EQUALS << m_uuid.GetStr();
	auto res = query.Exec();

	return (res.RowCount() == 1);
}

void ApoapseOperation::LogOperation(DbId associatedUserId)
{
	ASSERT_MSG(!m_operationId.is_initialized(), "The operation already have a database id which should not be know since we are attempting to save the operation on the database for the first time.");

	const string dir = (m_direction == OperationDirection::RECEIVED) ? "R" : "S";

	SQLQuery query(server.database);
	query << INSERT_INTO << "operations_log (operation, direction, item_id, user_id)" << VALUES << "(" << m_name << "," << dir << "," << m_itemId.get() << "," << associatedUserId << ")";
	//query << INSERT_INTO << "operations_log (timestamp, operation, direction, item_id, user_id)" << VALUES << "(" << TIMESTAMP << "," << m_name << "," << dir << "," << m_itemId.get() << "," << associatedUserId << ")";
	query.ExecAsync();

	LOG << "Logged new operation on the database for item " << m_name << ":" << m_uuid << " DbId: " << m_itemId.get() << LogSeverity::verbose;
}
