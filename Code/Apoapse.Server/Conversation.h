#pragma once
#include "ApoapseOperation.h"
class ApoapseAddress;

class Conversation final : public ApoapseOperation
{
	const std::vector<ApoapseAddress> m_correspondents;

public:
	Conversation(const Uuid& uuid, std::vector<ApoapseAddress>& correspondents, OperationDirection dir, ApoapseServer& serverRef);

	static UInt32 GetMaxAllowedCorespondants();
	
// 	static Conversation Create(const Command& command, Dirrection dir, /*Int64 userId*/);
// 	static Conversation Create(Int64 idOnDatabase);

	static Conversation Create(const Uuid& uuid, ApoapseServer& server);

private:
	DbId SaveToDatabaseInternal() override;
};