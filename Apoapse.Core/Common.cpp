#include "stdafx.h"
#include "Common.h"
#include "Logger.h"
#include "LogMessage.h"

Global* global = nullptr;

Global::Global() : settings(nullptr), logger(nullptr)
{

}

inline void Log(const string& msg, const LogSeverity severity)
{
	if (global && global->logger)
		global->logger->Log(msg, severity);
	else
	{
		auto tempLogMessage = LogMessage(msg, severity);
		tempLogMessage.LogToConsole();
	}
}

inline void FatalError(const string & msg)
{
	if (global && global->logger)
		global->logger->Log(msg, LogSeverity::fatalError, false);
	else
		Log(msg, LogSeverity::fatalError);	// Log to console only if the logger is not available

	std::abort();
}
