#include "stdafx.h"
#include "Common.h"
#include "ServerCmdManager.h"
#include "ServerConnection.h"
#include "CommandsDef.hpp"

ServerCmdManager::ServerCmdManager() : CommandsManagerV2(GetCommandDef())
{
	
}

bool ServerCmdManager::OnReceivedCommandPre(CommandV2& cmd, GenericConnection& connection)
{
	return true;
}

void ServerCmdManager::OnReceivedCommand(CommandV2& cmd, GenericConnection& netConnection)
{
	auto& connection = static_cast<ServerConnection&>(netConnection);

	LOG_DEBUG << "RECEIVED!";
}
