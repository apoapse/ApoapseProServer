#include "stdafx.h"
#include "ApoapseServer.h"
#include "Common.h"

#include "CommandsManager.h"
#include "Command.h"
#include "GenericConnection.h"


ApoapseServer::ApoapseServer(UInt16 port)
{
	m_ServerForClients = std::make_unique<TCPServer>(m_IOServiceForClients, port/*, TCPServer::IP_v6*/);
}

ApoapseServer::~ApoapseServer()
{
}

void ApoapseServer::Start()
{
	std::thread thread([this]
	{
		m_ServerForClients->StartAccept<GenericConnection>();
		m_IOServiceForClients.run();
	});
	thread.detach();
}
