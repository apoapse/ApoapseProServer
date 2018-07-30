#pragma once
#include "Command.h"
class User;

class CmdServerInfo : public Command
{
public:
	CommandInfo& GetInfo() const override;

	void SendSetupState(INetworkSender& destination);
	void SendWellcome(INetworkSender& destination, const User& user);
};