#include "stdafx.h"
#include "ApoapseCore.h"
#include "SettingsManager.h"
#include "JobManager.h"
#include "Logger.h"

void ApoapseCore::Start(Global* outsideGlobalPtr)
{
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

	Log("ApoapseCore started");
}

ApoapseCore::~ApoapseCore()
{

}