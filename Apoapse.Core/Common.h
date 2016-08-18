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
	SettingsManager* settings;
	ILogger* logger;

	Global();

	DLL_API static Global* CreateGlobal()
	{
		return new Global();
	}
};
extern Global* global;

inline DLL_API void Log(const string& msg, const LogSeverity severity = LogSeverity::normal);
inline DLL_API void FatalError(const string& msg);