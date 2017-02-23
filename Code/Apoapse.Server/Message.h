#pragma once
#include "ApoapseOperation.h"
#include "DateTimeUtils.h"
#include "Conversation.h"

using namespace DateTimeUtils;

class Message final : public ApoapseOperation
{
	const std::vector<byte>& m_messageBody;
	const Conversation m_conversation;
	UTCDateTime m_sentTime;
	const ApoapseAddress m_sender;

protected:
	static string databaseTableName;
	static string databaseOperationName;

public:
	Message(const Uuid& uuid, const ApoapseAddress& sender, Conversation& conversation, const UTCDateTime& sentTime, const std::vector<byte>& messageBody, OperationDirection dir, ApoapseServer& serverRef);
	
	//void Send(const LocalUser& user, GenericConnection& originConnection);

private:
	void SaveToDatabaseInternal() override;
};