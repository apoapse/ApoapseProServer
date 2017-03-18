#pragma once
#include "ApoapseOperation.h"
#include "ApoapseAddress.h"

class Conversation final : public ApoapseOperation
{
	const std::vector<ApoapseAddress> m_correspondents;

public:
	static string databaseTableName;
	static string databaseOperationName;

	Conversation(const Uuid& uuid, std::vector<ApoapseAddress>& correspondents, OperationDirection dir, ApoapseServer& serverRef);

	const std::vector<ApoapseAddress>& GetCorrespondents() const;
	static UInt32 GetMaxAllowedCorespondants();
	//void Send(const std::vector<ApoapseAddress>& recipients);
	
// 	static Conversation Create(const Command& command, Dirrection dir, /*Int64 userId*/);
// 	static Conversation Create(Int64 idOnDatabase);

	static std::shared_ptr<Conversation> Create(const Uuid& uuid, ApoapseServer& server);
	static std::shared_ptr<Conversation> Create(Int64 idOnDatabase, ApoapseServer& server, OperationDirection dir = OperationDirection::UNDEFINDED);

	std::unique_ptr<Command> PrepareCommandToBeSent() override;

private:
	void SaveToDatabaseInternal() override;
};