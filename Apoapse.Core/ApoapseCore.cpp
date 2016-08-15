#include "stdafx.h"
#include "ApoapseCore.h"

void ApoapseCore::Start(Global* inputGlobal)
{
	global = inputGlobal;
	ASSERT(global);

	global->logger->Init("log.txt");
	global->settings->Init("config.json");

	Log("ApoapseCore started");
}

ApoapseCore::~ApoapseCore()
{

}