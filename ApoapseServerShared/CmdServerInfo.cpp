#include "stdafx.h"
#include "CmdServerInfo.h"
#include "Common.h"
#include "CommandsManager.h"

void CmdServerInfo::SendSetupState(INetworkSender& destination)
{
	MessagePackSerializer ser;
	ser.UnorderedAppend<std::string>("status", "setup_state");

	Send(ser, destination);
}

APOAPSE_COMMAND_REGISTER(CmdServerInfo, CommandId::server_info);