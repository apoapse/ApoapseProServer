#include "stdafx.h"
#include "ApoapseServer.h"
#include "SettingsManager.h"
#include "ConfigVars.h"
#include "JobManager.h"
#include "Logger.h"

#include "TCPConnection.h"
#include "TestConnection.h"
#include "TCPServer.h"

#ifdef UNIT_TESTS
#include "UnitTestsSystem.h"
#endif // UNIT_TESTS

void ApoapseServer::Start(Global* outsideGlobalPtr, std::vector<std::string>& params)
{
#ifdef UNIT_TESTS
	if (std::find(params.begin(), params.end(), "-run_unit_tests") != params.end())
	{
		UnitTestsSystem::RegisterUnitTests();
		UnitTestsSystem::RunTests();
	}
#endif // UNIT_TESTS

	ASSERT(global == nullptr);

	global = outsideGlobalPtr;
	ASSERT(global);

	global->settings = new SettingsManager();
	try
	{
		global->settings->Init("config.json");
		ConfigVars::RegisterConfigVars();
	}
	catch (const std::exception&)
	{
		FatalError("Unable to load the config file config.json");
	}

	global->jobManager = new JobManager();
	global->jobManager->Init();

	global->logger = new Logger();
	global->logger->Init("log.txt");

	NetMessage::SetMaxAllowedSize();
	boost::asio::io_service io_serviceGeneral;

	//auto client = TCPConnection::Create<TestConnection>(io_serviceGeneral);
	//client->Connect("127.0.0.1", 55056);
	TCPServer server(io_serviceGeneral, 3000/*, TCPServer::IP_v6*/);
	server.StartAccept<TestConnection>();

	io_serviceGeneral.run();
}

ApoapseServer::~ApoapseServer()
{

}
