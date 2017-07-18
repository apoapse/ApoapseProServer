#include "stdafx.h"
#include "ServerEntryPoint.h"

#include "Logger.h"
#include "Common.h"
#include <iostream>
#include "ServerSettings.h"

#ifdef UNIT_TESTS
#include "UnitTestsManager.h"
#endif // UNIT_TESTS

int ServerMain(const std::vector<std::string>& launchArgs)
{
	ASSERT(global == nullptr);
	global = Global::CreateGlobal();

	global->logger = std::make_unique<Logger>("log.txt");
	global->threadPool = std::make_unique<ThreadPool>("Global thread pool", 8);

	global->settings = new ServerSettings();
	global->settings->Load("ServerConfig.ini");

	LOG << "server_port: " << global->settings->vars.server_port;

#ifdef UNIT_TESTS
	if (std::find(launchArgs.begin(), launchArgs.end(), "-run_unit_tests") != launchArgs.end())
	{
		UnitTestsManager::GetInstance().RunTests();
		return 0;
	}
#endif // UNIT_TESTS

	delete global->settings;
	return 0;
}