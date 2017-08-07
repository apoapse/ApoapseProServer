#include "stdafx.h"
#include "ServerConnection.h"
#include "Common.h"
#include "ApoapseServer.h"

ServerConnection::ServerConnection(boost::asio::io_service& ioService, ApoapseServer* server)
	: GenericConnection(ioService)
	, m_server(server)
{

}

ServerConnection::~ServerConnection()
{

}

bool ServerConnection::OnConnectedToServer()
{
	return true;
}

void ServerConnection::OnReceivedPayload(std::shared_ptr<NetworkPayload> payload)
{

}
