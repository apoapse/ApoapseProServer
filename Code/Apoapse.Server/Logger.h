#pragma once
#include "LogMessage.h"
#include <boost\thread\mutex.hpp>

class Logger : public ILogger
{
public:
	Logger();
	~Logger();
	void Init(const std::string& logFileLocation);
	void Log(const std::string& msg, LogSeverity severity = LogSeverity::normal, bool asyncLogToFile = true);
	std::string GetCurrentLogFileName();
	void WriteToLogFileRaw(const std::string& text);

private:
	std::string m_logFileLocation;
	boost::mutex m_mutex;
};