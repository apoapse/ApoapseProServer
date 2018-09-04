#pragma once
#include "Command.h"
#include "OperationObjects.h"
class User;

class CmdNotifyNewUser : public Command, public IOperationObject
{
public:
	CommandInfo& GetInfo() const override;

	void SendNotifyNewUser(INetworkSender& destination, const Username& username, const ApoapseMetadata& metadataAll);

	void SendFromDatabase(DbId id, INetworkSender& connection) override;
};