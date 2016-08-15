#pragma once

#ifdef WINDOWS
#include <windows.h>
#endif // WINDOWS

struct LogMessage
{
	LogMessage(const std::string& msg, LogSeverity severity);
	~LogMessage() {}

	const std::string message;
	const LogSeverity logSeverity;

	void LogToConsole() const;
	void LogToFile() const;

private:
	std::string m_logFormatedDateTime;
	std::string GenerateLocalDateTime() const;

#ifdef WINDOWS
	WORD GetConsoleColorBySeverity(LogSeverity) const;
#else
	std::string GetColorBySeverity(LogSeverity) const;
#endif
};
