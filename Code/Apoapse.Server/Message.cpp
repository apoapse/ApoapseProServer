#include "stdafx.h"
#include "Message.h"
#include "Common.h"
#include "CommandsManager.h"

string Message::databaseTableName = "messages";
string Message::databaseOperationName = "MESSAGE";

Message::Message(const Uuid& uuid, const ApoapseAddress& sender, Conversation& conversation, const UTCDateTime& sentTime, const std::vector<byte>& messageBody, OperationDirection dir, ApoapseServer& serverRef)
	: m_conversation(conversation),
	m_sender(sender),
	m_sentTime(sentTime),
	m_messageBody(messageBody),
	ApoapseOperation(uuid, databaseOperationName, databaseTableName, dir, serverRef)
{
}

// void Message::Send(const LocalUser& user, GenericConnection& originConnection)
// {
// 	std::unique_ptr<Command> cmd = CommandsManager::GetInstance().CreateCommand("MESSAGE");
// 	//TODO
// }

void Message::SaveToDatabaseInternal()
{
	SQLQuery query(server.database);
	query << INSERT_INTO << STR_TO_QUERY_SYNTAX(m_itemDBTableName) << " (uuid, conversation_id, sender, sent_time, msg_body)"	// #TODO use the contact system instead of storing the sender address
		  << VALUES << "(" << m_uuid.GetStr() << "," << m_conversation.GetItemDbId() << "," << m_sender.GetFullAddress() << "," << m_sentTime.str() << "," << m_messageBody << ")";
		 
	query.Exec();
}
