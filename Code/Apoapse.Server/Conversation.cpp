#include "stdafx.h"
#include "Conversation.h"
#include "Common.h"
#include "ApoapseAddress.h"

Conversation::Conversation(const Uuid& uuid, std::vector<ApoapseAddress>& correspondents, OperationDirection dir, ApoapseServer& serverRef)
	: ApoapseOperation(uuid, "CONVERSATION", "conversations", dir, serverRef),
	m_correspondents(std::move(correspondents))
{
}

DbId Conversation::SaveToDatabaseInternal()
{
	SQLQuery query(server.database);
 	query << INSERT_INTO << m_itemDBTableName.c_str() << " (uuid, correspondents)" << VALUES << "(" << m_uuid.GetStr() << "," << SQLQuery::FormatArray(m_correspondents) << ")";

	if (query.Exec())
	{
		SQLQuery queryId(server.database);
		queryId << SELECT << "id" << FROM << m_itemDBTableName.c_str() << " " << WHERE << "uuid" << EQUALS << m_uuid.GetStr();
		auto res = queryId.Exec();

		return res[0][0].GetInt64();
	}
	else
		return -1;
}

UInt32 Conversation::GetMaxAllowedCorespondants()
{
	return 24;
}
