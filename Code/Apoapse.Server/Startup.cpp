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

#include "ThreadPool.h"
#include "Log.h"

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

	global->logger = std::make_unique<Logger>("log.txt");
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

	auto threadPool = new ThreadPool("Main (test)", 4);
	auto test = threadPool->PushTask([] { std::this_thread::sleep_for(std::chrono::milliseconds(300)); return string("My name is NULL"); });
	//auto test2 = threadPool->Push([] { //Log("HELLLLLO!!!"); return 101; });

	
	
	
	NetMessage::SetMaxAllowedSize();	//TODO: change location
	//auto client = TCPConnection::Create<TestConnection>(io_serviceGeneral);
	//client->Connect("127.0.0.1", 55056);

	std::thread thread([&]
	{
		threadPool->PushTask([] {
			std::stringstream ss;
			ss << std::this_thread::get_id();
			string mystring = ss.str();

			std::this_thread::sleep_for(std::chrono::milliseconds(2000));

			//Log("Completed LONG on thread: " + mystring);
		});
		//Log("Quik1");


		boost::asio::io_service io_serviceGeneral;

		TCPServer server(io_serviceGeneral, 3000/*, TCPServer::IP_v6*/);
		server.StartAccept<TestConnection>();

		io_serviceGeneral.run(); 
	});
	thread.detach();

	threadPool->PushTask([] {
		std::stringstream ss;
		ss << std::this_thread::get_id();
		string mystring = ss.str();

		std::this_thread::sleep_for(std::chrono::milliseconds(100));

		//Log("Completed short on thread: " + mystring);
	});

	Log(*global->logger) << LogSeverity::debug << "HI" << " " << 15;

	LOG << "Works?!";
	LOG << LogSeverity::warning << "Oh yeah.";
	LOG_DEBUG_ONLY("yup" << " " << "that's correct");

	//Log("Quik2");
	//Log("NAME?: " + test.get());
	//io_serviceGeneral.run();

	string s;
	std::getline(std::cin, s);
}

ApoapseServerStartup::~ApoapseServerStartup()
{

}
