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
#define LOG if (global->logger.get() != nullptr) Log(*global->logger) << "[" << ExtractClassName(__FUNCTION__) << "] "

inline string ExtractClassName(const string& fullName)
{
	auto pos = fullName.find_last_of("::");

	if (pos != std::string::npos)
	{
		string ouput = fullName.substr(0, pos - 1);

		auto lambdaSpecifierPos = fullName.find("::<lambda_");
		if (lambdaSpecifierPos != std::string::npos)
			ouput = ouput.substr(0, lambdaSpecifierPos);

		return ouput;
	}
	else
		return fullName;
}