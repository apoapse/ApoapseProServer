#include "stdafx.h"
#include "ApoapseNetwork.h"
#include "ApoapseAddress.h"

#include "TCPServer.h"
#include "TCPConnection.h"
#include "TestConnection.h"
#include "NetMessage.h"
#include "Apoapse.Core\ByteUtils.h"

Global* global = nullptr;

void ApoapseNetwork::Start(Global* outsideGlobalPtr)
{
	ASSERT(global == nullptr);

	global = outsideGlobalPtr;
	ASSERT(global);

	Log("ApoapseNetwork started");


	byte bytes[] = { 69, 113, 33, 57 };
	//char* value = ByteConverter::ToChars(bytes, 3, 1);
	std::string test((char*)bytes + 0, 2);

	boost::asio::io_service io_serviceGeneral;

	TCPServer server(io_serviceGeneral, 3000, TCPServer::IP_v6);
	server.StartAccept<TestConnection>();

	//auto client = TCPConnection::Create<TestConnection>(io_serviceGeneral);
	//client->Connect("127.0.0.1", 55056);

	io_serviceGeneral.run();
	Log("This sould never be called", LogSeverity::error);
}

ApoapseNetwork::~ApoapseNetwork()
{

}