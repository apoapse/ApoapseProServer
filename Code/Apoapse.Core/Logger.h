#pragma once
#include "LogMessage.h"
#include <boost\thread\mutex.hpp>

class Logger : public ILogger
{
public:
	Logger();
	~Logger();
	void Init(const std::string&);
	void Log(const std::string&, LogSeverity = LogSeverity::normal, bool asyncLogToFile = true);
	std::string GetCurrentLogFileName();
	void WriteToLogFileRaw(const std::string&);

private:
	std::string m_logFileLocation;
	boost::mutex m_mutex;
};