#pragma once
#include "Types.h"
#include "Platforms.h"
#include "Diagnostics.h"

#include "ISettingsManager.h"
#include "IJobManager.h"
#include "Log.h"

struct Global
{
	ISettingsManager* settings = nullptr;
	IJobManager* jobManager = nullptr;
	//Logger* logger = nullptr;
	std::unique_ptr<Logger> logger;

	Global()
	{
		
	}

	DLL_API static Global* CreateGlobal()
	{
		return new Global();
	}
};
extern Global* global;

extern Logger* g_logger;

//DLL_API void Log(const string& msg, const LogSeverity severity = LogSeverity::normal);
DLL_API void FatalError(const string& msg);
#define LOG if (global->logger.get() != nullptr) Log(*global->logger)