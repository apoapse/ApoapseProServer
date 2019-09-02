#pragma once
#include "TypeDefs.hpp"
#include "CommandsManagerV2.h"
class User;

class ServerCmdManager : public CommandsManagerV2
{
public:
	ServerCmdManager();

	// Inherited via CommandsManagerV2
	bool OnSendCommandPre(CommandV2& cmd) override;
	bool OnReceivedCommandPre(CommandV2& cmd, GenericConnection& netConnection) override;
	void OnReceivedCommand(CommandV2& cmd, GenericConnection& netConnection) override;
	void OnReceivedCommandPost(CommandV2& cmd, GenericConnection& netConnection) override;
	void Propagate(CommandV2& cmd, GenericConnection& localConnection) override;
	void PropagateToUser(CommandV2& cmd, User& user);
};
