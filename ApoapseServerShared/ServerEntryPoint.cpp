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
#include "ServerCmdManager.h"
#include "ApoapseOpenSSL.h"
#include <filesystem>

#ifdef UNIT_TESTS
#include "UnitTest.hpp"
#endif // UNIT_TESTS

// UNIX STACK TRACE
#ifndef _WIN32
#include <stdio.h>
#include <execinfo.h>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>


void handler(int sig) {
  void *array[10];
  size_t size;

  // get void*'s for all entries on the stack
  size = backtrace(array, 10);

  // print out all the frames to stderr
  fprintf(stderr, "Error: signal %d:\n", sig);
  backtrace_symbols_fd(array, size, STDERR_FILENO);
  exit(1);
}
#endif

int ServerMain(const std::vector<std::string>& launchArgs)
{
#ifndef _WIN32
	signal(SIGSEGV, handler);	// Stack trace on linux
#endif


	// Starting global
	ASSERT(global == nullptr);
	global = Global::CreateGlobal();
	global->isServer = true;

	global->logger = std::make_unique<Logger>("log.txt");
	global->threadPool = std::make_unique<ThreadPool>("Global thread pool", 2, true);
	global->mainThread = std::make_unique<ThreadPool>("Main thread", 1, false);

	global->settings = new ServerSettings();
	global->settings->Load("ServerConfig.ini");

	global->apoapseData = std::make_unique<ApoapseData>(GetDataStructures());
	global->cmdManager = std::make_unique<ServerCmdManager>();

	if (!std::filesystem::exists("server.key") || !std::filesystem::exists("dh2048.pem"))
	{
		LOG << "The TLS keys are missing. Trying to auto-generate them using OpenSLL...";
		ApoapseOpenSSL::GenerateAllKeys();
	}

	// Database
	boost::shared_ptr<IDatabase> databaseSharedPtr = LibraryLoader::LoadLibrary<IDatabase>("DatabaseImpl.sqlite");
	global->database = databaseSharedPtr.get();
	databaseSharedPtr->Initialize();
	
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
		UnitTestsManager::GetInstance().RunTests(std::cout);
		return 0;
	}
#endif // UNIT_TESTS

	// Starting Apoapse server
	ApoapseServer server{};
	server.SetupMainServer(global->settings->vars.server_port);
	server.SetupFilesServer(global->settings->vars.server_port + 1);
	server.StartIOServices();

	// Make sure the console do not stop right away
	std::string inputstr;
	getline(std::cin, inputstr);

	// Cleanup
	delete global->settings;
	return 0;
}