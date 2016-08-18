#include "stdafx.h"
#include "ApoapseCore.h"

void ApoapseCore::Start(Global* inputGlobal)
{
	ASSERT(global == nullptr);

	global = inputGlobal;
	ASSERT(global);

	global->logger->Init("log.txt");
	global->settings->Init("config.json");

	Log("ApoapseCore started");
}

ApoapseCore::~ApoapseCore()
{

}