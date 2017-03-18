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
 	query << INSERT_INTO << STR_TO_QUERY_SYNTAX(m_itemDBTableName) << " (uuid, correspondents)" << VALUES << "(" << m_uuid.ToStr() << "," << SQLQuery::FormatArray(m_correspondents) << ")";

	query.Exec();
}

UInt32 Conversation::GetMaxAllowedCorespondants()
{
	return 24;
}

std::shared_ptr<Conversation> Conversation::Create(const Uuid& uuid, ApoapseServer& server)
{
	SQLQuery query(server.database);
	query << SELECT << ALL << FROM << STR_TO_QUERY_SYNTAX(databaseTableName) << WHERE << "uuid" << EQUALS << uuid.ToStr();
	auto res = query.Exec();

	if (res.RowCount() == 0)
		throw std::out_of_range("The requested conversation uuid does not exist in the database");

	const Int64 convId = res[0][COLUMN_NAME_HELPER("id", 0)].GetInt64();
	const auto operationInfo = GetOperationInfoFromDatabase(convId, databaseOperationName, server);

	std::vector<ApoapseAddress> correspondents;
	auto correspondentsStrArray = SQLQuery::TextToArray(res[0][COLUMN_NAME_HELPER("correspondents", 2)].GetText());

	for (const auto& addressStr : correspondentsStrArray)
		correspondents.emplace_back(addressStr);

	auto conv = std::make_shared<Conversation>(uuid, correspondents, operationInfo.direction, server);
	conv->SetItemDbId(convId);

	return conv;
}

std::shared_ptr<Conversation> Conversation::Create(Int64 idOnDatabase, ApoapseServer& server, OperationDirection dir/* = OperationDirection::UNDEFINDED*/)
{
	SQLQuery query(server.database);
	query << SELECT << ALL << FROM << STR_TO_QUERY_SYNTAX(databaseTableName) << WHERE << "id" << EQUALS << idOnDatabase;
	auto res = query.Exec();

	if (res.RowCount() == 0)
		throw std::out_of_range("The requested conversation id does not exist in the database");

	std::vector<ApoapseAddress> correspondents;
	const auto correspondentsStrArray = SQLQuery::TextToArray(res[0][COLUMN_NAME_HELPER("correspondents", 2)].GetText());
	const auto uuid = Uuid(res[0][1].GetText());

	for (const auto& addressStr : correspondentsStrArray)
		correspondents.emplace_back(addressStr);

	auto conv = std::make_shared<Conversation>(uuid, correspondents, dir, server);
	conv->SetItemDbId(idOnDatabase);

	return conv;
}

const std::vector<ApoapseAddress>& Conversation::GetCorrespondents() const
{
	return m_correspondents;
}

std::unique_ptr<Command> Conversation::PrepareCommandToBeSent()
{
	std::unique_ptr<Command> cmd = CommandsManager::GetInstance().CreateCommand("CONVERSATION");
	cmd->InsertFieldValue("uuid", m_uuid.ToStr());
	cmd->InsertFieldArray("correspondents", m_correspondents);

	return cmd;
}
