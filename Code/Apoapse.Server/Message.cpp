#include "stdafx.h"
#include "Message.h"
#include "Common.h"
#include "CommandsManager.h"

string Message::databaseTableName = "messages";
string Message::databaseOperationName = "MESSAGE";

Message::Message(const Uuid& uuid, const ApoapseAddress& sender, std::shared_ptr<Conversation> conversation, const UTCDateTime& sentTime, std::shared_ptr<std::vector<byte>> messageBody, OperationDirection dir, ApoapseServer& serverRef)
	: m_conversation(conversation)
	, m_sender(sender)
	, m_sentTime(sentTime)
	, m_messageBody(messageBody)
	, ApoapseOperation(uuid, databaseOperationName, databaseTableName, dir, serverRef)
{
}

std::unique_ptr<Command> Message::PrepareCommandToBeSent()
{
	std::unique_ptr<Command> cmd = CommandsManager::GetInstance().CreateCommand("MESSAGE");
	cmd->SetPayload(m_messageBody);
	cmd->InsertFieldValue<string>("uuid", m_uuid.ToStr());
	if (m_sender.GetFullAddress() != ":") cmd->InsertFieldValue<string>("from", m_sender.GetFullAddress());	// If the sender full address is empty, GetFullAddress() can return ":" #TODO Should we allow empty "from" field? Automaticaly fill with the current user address?
	cmd->InsertFieldValue<string>("conversation", m_conversation->GetItemUuid().ToStr());
	cmd->InsertFieldValue<string>("sent", m_sentTime.str());

	return cmd;
}

void Message::SaveToDatabaseInternal()
{
	SQLQuery query(server.database);
	query << INSERT_INTO << STR_TO_QUERY_SYNTAX(m_itemDBTableName) << " (uuid, conversation_id, sender, sent_time, msg_body)"	// #TODO use the contact system instead of storing the sender address
		  << VALUES << "(" << m_uuid.ToStr() << "," << m_conversation->GetItemDbId() << "," << m_sender.GetFullAddress() << "," << m_sentTime.str() << "," << *m_messageBody << ")";
		 
	query.Exec();
}

const std::vector<ApoapseAddress>& Message::GetCorrespondents() const
{
	return m_conversation->GetCorrespondents();
}

Message Message::Create(Int64 idOnDatabase, ApoapseServer& server, OperationDirection dir /*= OperationDirection::UNDEFINDED*/)
{
	SQLQuery query(server.database);
	query << SELECT << ALL << FROM << STR_TO_QUERY_SYNTAX(databaseTableName) << WHERE << "id" << EQUALS << idOnDatabase;
	auto res = query.Exec();

	if (res.RowCount() == 0)
		throw std::out_of_range("The requested message id does not exist in the database");

	const auto uuid = Uuid(res[0][1].GetText());
	const DbId convId = res[0][2].GetInt64();
	const ApoapseAddress sender = ApoapseAddress(res[0][3].GetText());
	const UTCDateTime sentTime = UTCDateTime(res[0][4].GetText());
	const auto messageBody = std::make_shared<std::vector<byte>>(res[0][5].GetByteArray());

	auto conv = Conversation::Create(convId, server);

	Message message(uuid, sender, conv, sentTime, messageBody, dir, server);
	message.SetItemDbId(idOnDatabase);

	return message;
}
