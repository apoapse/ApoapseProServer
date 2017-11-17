#pragma once
#include "Command.h"

class CmdServerInfo : public Command
{
public:
	CommandInfo& GetInfo() const override;

	void SendSetupState(INetworkSender& destination);
};