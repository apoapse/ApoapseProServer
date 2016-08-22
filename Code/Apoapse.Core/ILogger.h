#pragma once
enum LogSeverity
{
	normal,
	verbose,
	debug,
	warning,
	error,
	fatalError	// Warning: this will only log a message as fatal error. Use FatalError()
};

struct ILogger
{
	virtual void Init(const std::string& logFileLocation) = 0;
	virtual void Log(const std::string& msg, LogSeverity severity = LogSeverity::normal, bool asyncLogToFile = true) = 0;
	virtual std::string GetCurrentLogFileName() = 0;
	virtual void WriteToLogFileRaw(const std::string& text) = 0;
};