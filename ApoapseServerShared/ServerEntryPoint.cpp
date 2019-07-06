#include "stdafx.h"
#include "ServerEntryPoint.h"

#include "Logger.h"
#include "Common.h"
#include "ServerSettings.h"
#include "ApoapseServer.h"
#include "Database.hpp"
#include "LibraryLoader.hpp"
#include "DatabaseIntegrityPatcher.h"
#include "DataStructures.hpp"
#include "CommandsManagerV2.h"

#ifdef UNIT_TESTS
#include "UnitTestsManager.h"
#endif // UNIT_TESTS
#include <Uuid.h>
#include <CommandsDef.hpp>

int ServerMain(const std::vector<std::string>& launchArgs)
{
	// Starting global
	ASSERT(global == nullptr);
	global = Global::CreateGlobal();
	global->isServer = true;

	global->logger = std::make_unique<Logger>("log.txt");
	global->threadPool = std::make_unique<ThreadPool>("Global thread pool", 8);

	global->settings = new ServerSettings();
	global->settings->Load("ServerConfig.ini");

	global->apoapseData = std::make_unique<ApoapseData>(GetDataStructures());
	global->cmdManager = std::make_unique<CommandsManagerV2>(GetCommandDef());

	// Database
	boost::shared_ptr<IDatabase> databaseSharedPtr = LibraryLoader::LoadLibrary<IDatabase>("DatabaseImpl.sqlite");
	global->database = databaseSharedPtr.get();
	const char* dbParams[1];
	dbParams[0] = "server_database.db";
	if (databaseSharedPtr->Open(dbParams, 1))
	{
		LOG << "Database accessed successfully. Params: " << *dbParams;

		DatabaseIntegrityPatcher dbIntegrity;
		if (!dbIntegrity.CheckAndResolve())
		{
			FatalError("The database integrity patcher has failed");
		}
	}
	else
	{
		FatalError("Unable to access the database");
	}

#ifdef UNIT_TESTS
	if (std::find(launchArgs.begin(), launchArgs.end(), "-run_unit_tests") != launchArgs.end())
	{
		UnitTestsManager::GetInstance().RunTests();
		return 0;
	}
#endif // UNIT_TESTS

	// Starting Apoapse server
	ApoapseServer server{};
	server.StartMainServer(global->settings->vars.server_port);

	// Make sure the console do not stop right away
	std::string inputstr;
	getline(std::cin, inputstr);

	// Cleanup
	delete global->settings;
	return 0;
}