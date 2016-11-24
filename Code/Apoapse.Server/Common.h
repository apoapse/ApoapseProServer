#pragma once
#include <memory>

#include "Types.h"
#include "Platforms.h"
#include "Diagnostics.h"

#include "ISettingsManager.h"
#include "ThreadPool.h"
#include "ILogger.h"
#include "Log.h"

struct Global
{
	std::unique_ptr<ISettingsManager> settings;
	std::unique_ptr<ThreadPool> threadPool;
	std::unique_ptr<ILogger> logger;

	Global()
	{
	}

	static Global* CreateGlobal()
	{
		return new Global();
	}
};
extern Global* global;

void FatalError(const string& msg);
#define LOG if (global->logger.get() != nullptr) Log(*global->logger)