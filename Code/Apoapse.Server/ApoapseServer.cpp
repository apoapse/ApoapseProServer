#include "stdafx.h"
#include "ApoapseServer.h"
#include "SettingsManager.h"
#include "JobManager.h"
#include "Logger.h"

#ifdef UNIT_TESTS
#include "UnitTestsSystem.h"
#endif // UNIT_TESTS

void ApoapseServer::Start(Global* outsideGlobalPtr)
{
#ifdef UNIT_TESTS
	UnitTestsSystem::RegisterUnitTests();
	UnitTestsSystem::RunTests();
#endif // UNIT_TESTS

	ASSERT(global == nullptr);

	global = outsideGlobalPtr;
	ASSERT(global);

	global->settings = new SettingsManager();
	try
	{
		global->settings->Init("config.json");
	}
	catch (const std::exception&)
	{
		FatalError("Unable to load the config file config.json");
	}

	global->jobManager = new JobManager();
	global->jobManager->Init();

	global->logger = new Logger();
	global->logger->Init("log.txt");

	Log("Apoapse.Server started");
}

ApoapseServer::~ApoapseServer()
{

}
