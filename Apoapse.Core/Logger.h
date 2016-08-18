#pragma once
#include "LogMessage.h"
#include "ProducerConsumerQueue.h"
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
	ProducerConsumerQueue* m_threadQueue;
	boost::mutex m_mutex;
};