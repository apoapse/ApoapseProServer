#include "stdafx.h"
#include "ApoapseServer.h"
#include "Common.h"

#include "CommandsManager.h"
#include "Command.h"
#include "GenericConnection.h"


ApoapseServer::ApoapseServer(UInt16 port) : m_tcpServer()
{
	m_tcpServer = std::make_unique<TCPServer>(m_IOServiceGeneral, port/*, TCPServer::IP_v6*/);

	








//   	auto test = CommandsManager::GetInstance().CreateCommand("CONNECT");
// 	auto test2 = test->CanProcessWithThisActor(new GenericConnection(m_IOServiceGeneral));
// 	//std::string test2 = u8"CONNECT\n{\"username\": \"Guillaume\",	\"password\" : \"MyPassword\"}";
// 	std::string test2 = u8"CONNECT\n{\"username\": 7,	\"password\" : \"MyPassword\"}";
// 	
// 	test->FromRawCmd(test2);
}

ApoapseServer::~ApoapseServer()
{
}

void ApoapseServer::Start()
{
	std::thread thread([this]
	{
		m_tcpServer->StartAccept<GenericConnection>();
		m_IOServiceGeneral.run();
	});
	thread.detach();
}
