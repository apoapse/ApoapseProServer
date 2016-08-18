#include "stdafx.h"
#include "ApoapseCore.h"
#include "Logger.h"

void ApoapseCore::Start(Global* inputGlobal)
{
	ASSERT(global == nullptr);

	global = inputGlobal;
	ASSERT(global);

	global->settings = new SettingsManager();
	global->settings->Init("config.json");

	global->logger = new Logger();
	global->logger->Init("log.txt");

	Log("ApoapseCore started");
}

ApoapseCore::~ApoapseCore()
{

}