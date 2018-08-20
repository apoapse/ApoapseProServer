#include "stdafx.h"
#include "OperationObjects.h"
#include "Common.h"
#include "SQLQuery.h"
#include "ServerConnection.h"

std::unique_ptr<IOperationObject> OperationObjects::CreateObject(OperationType operation)
{
	ASSERT(DoesObjectExist(operation));

	return operationObjects.CreateObject(operation);
}

bool OperationObjects::DoesObjectExist(OperationType operation) const
{
	return operationObjects.DoesObjectExist(operation);
}

void OperationObjects::SynchronizeSince(Int64 sinceTimestamp, ServerConnection& connection)
{
	LOG << "User " << connection.GetRelatedUser()->GetUsername().ToStr() << " requesting operations sync since " << sinceTimestamp;
	const Int64 sinceTimestampReal = (sinceTimestamp - 1);	// We subtract one second to make sure there is no operations that can be lost if there is delay between user disconnection for instance

	SQLQuery query(*global->database);
	query << SELECT << "type,item_dbid" << FROM << "operations" << WHERE "time" << GREATER_THAN << sinceTimestampReal << ORDER_BY << "time" << ASC;	// #MVP make sure the user is allowed to receive the command in question
	auto res = query.Exec();

	UInt64 i = 0;
	for (const auto& row : res)	// #THREADING add multi threading
	{
		OperationType type = static_cast<OperationType>(row[0].GetInt32());
		DbId dbId = row[1].GetInt64();

		if (GetInstance().DoesObjectExist(type))
		{
			auto operationObj = GetInstance().CreateObject(type);
			operationObj->SendFromDatabase(dbId, connection);

			i++;
		}
	}

	LOG << i << " operations synced to user " << connection.GetRelatedUser()->GetUsername().ToStr();
}
