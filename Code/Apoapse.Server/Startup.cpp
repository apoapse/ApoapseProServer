#include "stdafx.h"
#include "Startup.h"
#include "SettingsManager.h"
#include "ConfigVars.h"
#include "Logger.h"
#include "ApoapseServer.h"

#include "InternalLibraryLoader.h"
#include "Database.h"

#ifdef UNIT_TESTS
#include "Apoapse.UnitTest\TestsManager.h"
#endif // UNIT_TESTS

void ApoapseServerStartup::Start(std::vector<std::string>& params)
{
	ASSERT(global == nullptr);
	global = Global::CreateGlobal();

#ifdef UNIT_TESTS
	if (std::find(params.begin(), params.end(), "-run_unit_tests") != params.end())
	{
		TestsManager::GetInstance().RunTests();
		return;
	}
#endif // UNIT_TESTS

	try
	{
		global->settings = std::make_unique<SettingsManager>("config.json");
		ConfigVars::RegisterConfigVars();
	}
	catch (const std::exception&)
	{
		FatalError("Unable to load the config file config.json");
	}

	global->logger = std::make_unique<Logger>("log.txt");
	global->threadPool = std::make_unique<ThreadPool>("Global thread pool", global->settings->ReadConfigValue_uint("system.glboal_threadpool_nb_threads"));

	// Database
	boost::shared_ptr<Database> database = InternalLibraryLoader::LoadInternalLibrary<Database>("DatabaseImpl.sqlite");
	const char* dbParams[1];
	dbParams[0] = "database.db";
	if (!database->Open(dbParams, 1))
	{
		FatalError("Unable to access the database");
	}

	// Apoapse Server
	/*new*/ ApoapseServer apoapseServer((UInt16)global->settings->ReadConfigValue_uint("server.clients_port"), (UInt16)global->settings->ReadConfigValue_uint("server.servers_port"), *database.get());
	apoapseServer.Start();
	
	string s;
	std::getline(std::cin, s);
}

ApoapseServerStartup::~ApoapseServerStartup()
{

}
