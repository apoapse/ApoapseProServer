#include "stdafx.h"
#include "Conversation.h"
#include "Common.h"
#include "CommandsManager.h"

string Conversation::databaseTableName = "conversations";
string Conversation::databaseOperationName = "CONVERSATION";

Conversation::Conversation(const Uuid& uuid, std::vector<ApoapseAddress>& correspondents, OperationDirection dir, ApoapseServer& serverRef)
	: ApoapseOperation(uuid, databaseOperationName, databaseTableName, dir, serverRef),
	m_correspondents(std::move(correspondents))
{
	ASSERT(m_correspondents.size() > 0);
}

void Conversation::SaveToDatabaseInternal()
{
	SQLQuery query(server.database);
 	query << INSERT_INTO << STR_TO_QUERY_SYNTAX(m_itemDBTableName) << " (uuid, correspondents)" << VALUES << "(" << m_uuid.GetStr() << "," << SQLQuery::FormatArray(m_correspondents) << ")";

	query.Exec();
}

UInt32 Conversation::GetMaxAllowedCorespondants()
{
	return 24;
}

Conversation Conversation::Create(const Uuid& uuid, ApoapseServer& server)
{
	SQLQuery query(server.database);
	query << SELECT << ALL << FROM << STR_TO_QUERY_SYNTAX(databaseTableName) << WHERE << "uuid" << EQUALS << uuid.GetStr();
	auto res = query.Exec();

	const Int64 convId = res[0][COLUMN_NAME_HELPER("id", 0)].GetInt64();
	const auto operationInfo = GetOperationInfoFromDatabase(convId, databaseOperationName, server);

	std::vector<ApoapseAddress> correspondents;
	auto correspondentsStrArray = SQLQuery::TextToArray(res[0][COLUMN_NAME_HELPER("correspondents", 2)].GetText());

	for (const auto& addressStr : correspondentsStrArray)
		correspondents.push_back(ApoapseAddress(addressStr));

	Conversation conv(uuid, correspondents, operationInfo.direction, server);
	conv.SetItemDbId(convId);

	return conv;
}

const std::vector<ApoapseAddress>& Conversation::GetCorrespondents() const
{
	return m_correspondents;
}

std::unique_ptr<Command> Conversation::PrepareCommandToBeSent()
{
	std::unique_ptr<Command> cmd = CommandsManager::GetInstance().CreateCommand("CONVERSATION");
	//TODO

	return cmd;
}
