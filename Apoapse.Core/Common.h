#pragma once
#include "Platforms.h"
#include "Diagnostics.h"

#include <string>
#include <memory>
using string = std::string;

#include "ILogger.h"
#include "SettingsManager.h"

struct Global
{
	ILogger* logger;
	SettingsManager* settings;

	Global();

	DLL_API static Global* CreateGlobal()
	{
		return new Global();
	}
};

extern Global* global;


inline void Log(const string& msg, const LogSeverity severity = LogSeverity::normal)
{
	if (global && global->logger)
		global->logger->Log(msg, severity);
}

