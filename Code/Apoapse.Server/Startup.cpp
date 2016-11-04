#include "stdafx.h"
#include "Startup.h"
#include "SettingsManager.h"
#include "ConfigVars.h"
#include "JobManager.h"
#include "Logger.h"
#include "ApoapseServer.h"

#include "TCPConnection.h"
#include "TestConnection.h"
#include "TCPServer.h"

#include <thread>
#include "Command.h"
#include "CommandsManager.h"
#include <boost\any.hpp>

#ifdef UNIT_TESTS
#include "UnitTestsSystem.h"
#endif // UNIT_TESTS

void ApoapseServerStartup::Start(Global* outsideGlobalPtr, std::vector<std::string>& params)
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
	
	
	NetMessage::SetMaxAllowedSize();	//TODO: change location
	//auto client = TCPConnection::Create<TestConnection>(io_serviceGeneral);
	//client->Connect("127.0.0.1", 55056);

	std::thread thread([]
	{
		boost::asio::io_service io_serviceGeneral;

		TCPServer server(io_serviceGeneral, 3000/*, TCPServer::IP_v6*/);
		server.StartAccept<TestConnection>();

		io_serviceGeneral.run(); 
	});
	thread.detach();

	//io_serviceGeneral.run();
}

ApoapseServerStartup::~ApoapseServerStartup()
{

}
