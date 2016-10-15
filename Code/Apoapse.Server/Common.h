#pragma once
#include "Types.h"
#include "Platforms.h"
#include "Diagnostics.h"

#include <boost/format.hpp>

#include "ISettingsManager.h"
#include "IJobManager.h"
#include "ILogger.h"

struct Global
{
	ISettingsManager* settings = nullptr;
	IJobManager* jobManager = nullptr;
	ILogger* logger = nullptr;

	Global()
	{
	}

	DLL_API static Global* CreateGlobal()
	{
		return new Global();
	}
};
extern Global* global;

DLL_API void Log(const string& msg, const LogSeverity severity = LogSeverity::normal);
DLL_API void FatalError(const string& msg);

template<typename... Arguments>
string Format(const string& inputStr, Arguments&&... args)
{
	boost::format formated(inputStr);
	int unroll[]{ 0, (formated % std::forward<Arguments>(args), 0)... };
	static_cast<void>(unroll);

	return boost::str(formated);
}