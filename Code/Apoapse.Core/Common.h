#pragma once
#include "Platforms.h"
#include "Diagnostics.h"

#include <string>
#include <memory>
using string = std::string;

#include "ISettingsManager.h"
#include "IJobManager.h"
#include "ILogger.h"

struct Global
{
	ISettingsManager* settings;
	IJobManager* jobManager;
	ILogger* logger;

	Global() : settings(nullptr), jobManager(nullptr), logger(nullptr)
	{
	}

	DLL_API static Global* CreateGlobal()
	{
		return new Global();
	}
};
extern Global* global;

inline DLL_API void Log(const string& msg, const LogSeverity severity = LogSeverity::normal);
inline DLL_API void FatalError(const string& msg);