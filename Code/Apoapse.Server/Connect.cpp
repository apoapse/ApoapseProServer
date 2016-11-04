#include "stdafx.h"
#include "Connect.h"
#include "Common.h"
#include "CommandsManager.h"


class Connect : public Command
{
	void SayHi()
	{
		Log("HI FROM Connect!");
	}
};
/*
void Connect::SayHi()
{
	Log("HI FROM Connect H!");
}*/

APOAPSE_COMMAND_REGISTER(Connect, "Connect");