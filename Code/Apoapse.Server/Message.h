#pragma once
#include "ApoapseOperation.h"
#include "DateTimeUtils.h"

using namespace DateTimeUtils;

class Message final : public ApoapseOperation
{
	const std::vector<byte>& m_messageBody;
	const Uuid m_conversationUuid;
	const UTCDateTime m_sentTime;

public:
	Message(const Uuid& uuid, const Uuid& conversation, const UTCDateTime& sentTime, const std::vector<byte>& messageBody, OperationDirection dir, ApoapseServer& serverRef);
	
private:
};