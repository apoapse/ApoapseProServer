#include "stdafx.h"
#include "Message.h"
#include "Common.h"
#include "CommandsManager.h"

string Message::databaseTableName = "messages";
string Message::databaseOperationName = "MESSAGE";

Message::Message(const Uuid& uuid, const ApoapseAddress& sender, Conversation& conversation, const UTCDateTime& sentTime, std::shared_ptr<std::vector<byte>> messageBody, OperationDirection dir, ApoapseServer& serverRef)
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
	cmd->InsertFieldValue<string>("uuid", m_uuid.GetStr());
	cmd->InsertFieldValue<string>("from", m_sender.GetFullAddress());
	cmd->InsertFieldValue<string>("conversation", m_conversation.GetItemUuid().GetStr());
	cmd->InsertFieldValue<string>("sent", m_sentTime.str());
	auto test = m_sender.GetFullAddress();
	return cmd;
}

void Message::SaveToDatabaseInternal()
{
	SQLQuery query(server.database);
	query << INSERT_INTO << STR_TO_QUERY_SYNTAX(m_itemDBTableName) << " (uuid, conversation_id, sender, sent_time, msg_body)"	// #TODO use the contact system instead of storing the sender address
		  << VALUES << "(" << m_uuid.GetStr() << "," << m_conversation.GetItemDbId() << "," << m_sender.GetFullAddress() << "," << m_sentTime.str() << "," << *m_messageBody << ")";
		 
	query.Exec();
}

const std::vector<ApoapseAddress>& Message::GetCorrespondents() const
{
	return m_conversation.GetCorrespondents();
}
