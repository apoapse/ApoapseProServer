#pragma once
#include "ApoapseOperation.h"
#include "ApoapseAddress.h"

class Conversation final : public ApoapseOperation
{
	const std::vector<ApoapseAddress> m_correspondents;

protected:
	static string databaseTableName;
	static string databaseOperationName;

public:
	Conversation(const Uuid& uuid, std::vector<ApoapseAddress>& correspondents, OperationDirection dir, ApoapseServer& serverRef);

	const std::vector<ApoapseAddress>& GetCorrespondents() const;
	static UInt32 GetMaxAllowedCorespondants();
	//void Send(const std::vector<ApoapseAddress>& recipients);
	
// 	static Conversation Create(const Command& command, Dirrection dir, /*Int64 userId*/);
// 	static Conversation Create(Int64 idOnDatabase);

	static Conversation Create(const Uuid& uuid, ApoapseServer& server);

private:
	void SaveToDatabaseInternal() override;
};