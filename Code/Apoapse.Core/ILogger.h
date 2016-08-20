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
	virtual void Init(const std::string&) = 0;
	virtual void Log(const std::string&, LogSeverity = LogSeverity::normal, bool asyncLogToFile = true) = 0;
	virtual std::string GetCurrentLogFileName() = 0;
	virtual void WriteToLogFileRaw(const std::string&) = 0;
};