#include "stdafx.h"
#include "ApoapseNetwork.h"
#include "ApoapseAddress.h"

#include "TCPServer.h"
#include "TCPConnection.h"
#include "TestConnection.h"

Global* global = nullptr;

void ApoapseNetwork::Start(Global* outsideGlobalPtr)
{
	ASSERT(global == nullptr);

	global = outsideGlobalPtr;
	ASSERT(global);

	Log("ApoapseNetwork started");

	boost::asio::io_service io_serviceGeneral;

	TCPServer server(io_serviceGeneral, 3000, TCPServer::IP_v6);
	server.StartAccept<TestConnection>();

	//auto client = TCPConnection::Create<TestConnection>(io_serviceGeneral);
	//client->Connect("127.0.0.1", 55056);

	io_serviceGeneral.run();
}

ApoapseNetwork::~ApoapseNetwork()
{

}