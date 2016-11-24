#include "stdafx.h"
#include "Common.h"
#include "CommandsManager.h"

class Connect : public Command
{
	void SayHi()
	{
		LOG << "HI FROM Connect!";
	}
};

APOAPSE_COMMAND_REGISTER(Connect, "Connect");