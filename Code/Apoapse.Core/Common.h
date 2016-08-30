#pragma once
#include "Platforms.h"
#include "Diagnostics.h"

#include <string>
#include <boost/format.hpp>
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

template<typename... Arguments>
string Format(const string& inputStr, Arguments&&... args)
{
	boost::format formated(inputStr);
	int unroll[]{ 0, (formated % std::forward<Arguments>(args), 0)... };
	static_cast<void>(unroll);

	return boost::str(formated);
}