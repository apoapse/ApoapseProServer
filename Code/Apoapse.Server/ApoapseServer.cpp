#include "stdafx.h"
#include "ApoapseServer.h"
#include "Common.h"

#include "CommandsManager.h"
#include "NetMessage.h"
#include "TCPServer.h"

ApoapseServer::ApoapseServer(UInt16 port)
{
	std::unique_ptr<MyStruct> test_sttuc = std::make_unique<MyStruct>();

	Test(std::move(test_sttuc));

	auto test = CommandsManager::GetInstance().CreateCommand("CONNECT", *this);
	std::string test2 = u8"CONNECT\n{\"username\": \"Guillaume\",	\"password\" : \"MyPassword\"}\n\nPAYLOAD";
	
	test->FromRawCmd(test2);

	//std::string test4 = u8"CONNECT\nGuillaume MyPassword\n";
	//auto test3 = CommandsManager::GetInstance().CreateCommand("CONNECT", *this);
	//test3->FromRawCmd(test4);

	

	NetMessage::SetMaxAllowedSize();
}

ApoapseServer::~ApoapseServer()
{
}

void ApoapseServer::Start()
{

}
