#pragma once
#include "ApoapseOperation.h"
#include "DateTimeUtils.h"
#include "Conversation.h"

using namespace DateTimeUtils;

class Message final : public ApoapseOperation
{
	std::shared_ptr<std::vector<byte>> m_messageBody;
	std::shared_ptr<Conversation> m_conversation;
	UTCDateTime m_sentTime;
	ApoapseAddress m_sender;

public:
	static string databaseTableName;
	static string databaseOperationName;

	Message(const Uuid& uuid, const ApoapseAddress& sender, std::shared_ptr<Conversation> conversation, const UTCDateTime& sentTime, std::shared_ptr<std::vector<byte>> messageBody, OperationDirection dir, ApoapseServer& serverRef);
	const std::vector<ApoapseAddress>& GetCorrespondents() const;
	std::unique_ptr<Command> PrepareCommandToBeSent() override;

	static Message Create(Int64 idOnDatabase, ApoapseServer& server, OperationDirection dir = OperationDirection::UNDEFINDED);

private:
	void SaveToDatabaseInternal() override;
};