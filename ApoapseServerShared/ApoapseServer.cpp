#include "stdafx.h"
#include "ApoapseServer.h"
#include "Common.h"
#include "ServerConnection.h"
#include "CommandsManager.h"

void ApoapseServer::StartMainServer(UInt16 port)
{
	m_mainServer = std::make_unique<TCPServer>(m_mainServerIOService, port, TCPServer::Protocol::ip_v6);
	usersManager = std::make_unique<UsersManager>();

	std::thread threadMainServer([this]
	{
		m_mainServer->StartAccept<ServerConnection>(this);
		m_mainServerIOService.run();
	});
	threadMainServer.detach();
}
