#pragma once
#include "ApoapseOperation.h"
#include "DateTimeUtils.h"
#include "Conversation.h"

using namespace DateTimeUtils;

class Message final : public ApoapseOperation
{
	std::shared_ptr<std::vector<byte>> m_messageBody;
	const Conversation& m_conversation;
	UTCDateTime m_sentTime;
	ApoapseAddress m_sender;

protected:
	static string databaseTableName;
	static string databaseOperationName;

public:
	Message(const Uuid& uuid, const ApoapseAddress& sender, Conversation& conversation, const UTCDateTime& sentTime, std::shared_ptr<std::vector<byte>> messageBody, OperationDirection dir, ApoapseServer& serverRef);
	const std::vector<ApoapseAddress>& GetCorrespondents() const;

private:
	void SaveToDatabaseInternal() override;
	std::unique_ptr<Command> PrepareCommandToBeSent() override;
};