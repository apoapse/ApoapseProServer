#include "stdafx.h"
#include "ApoapseServer.h"
#include "Common.h"

#include "CommandsManager.h"
#include "NetMessage.h"
#include "TCPServer.h"

ApoapseServer::ApoapseServer(UInt16 port)
{
	auto test = CommandsManager::GetInstance().CreateCommand("CONNECT", *this);
	std::string test2 = u8"CONNECT{\"username\": \"Guillaume\",	\"password\" : \"MyPassword\"}";
	
	//test->FromRawCmd(test2);

	std::string test4 = u8"CONNECT\nGuillaume MyPassword";
	auto test3 = CommandsManager::GetInstance().CreateCommand("CONNECT", *this);
	test3->FromRawCmd(test4);


	NetMessage::SetMaxAllowedSize();
}

ApoapseServer::~ApoapseServer()
{
}