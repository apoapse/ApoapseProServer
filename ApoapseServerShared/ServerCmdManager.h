#pragma once
#include "TypeDefs.hpp"
#include "CommandsManagerV2.h"
class User;

class ServerCmdManager : public CommandsManagerV2
{
public:
	ServerCmdManager();

	// Inherited via CommandsManagerV2
	virtual bool OnReceivedCommandPre(CommandV2& cmd, GenericConnection& netConnection) override;
	virtual void OnReceivedCommand(CommandV2& cmd, GenericConnection& netConnection) override;
	virtual void OnReceivedCommandPost(CommandV2& cmd, GenericConnection& netConnection) override;
	virtual void Propagate(CommandV2& cmd, GenericConnection& localConnection) override;
	void PropagateToUser(CommandV2& cmd, User& user);
};
