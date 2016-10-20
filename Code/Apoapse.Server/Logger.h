#pragma once
#include "LogMessage.h"
#include <boost\thread\mutex.hpp>
#include <chrono>

#define SPAM_PREVENTION_MAX_LOCK_DURATION 1500

class Logger : public ILogger
{
	std::string m_logFileLocation;
	boost::mutex m_mutex;
	bool m_logLock;
	int m_timedLogCount;	// Track the number of log calls for each SPAM_PREVENTION_MAX_LOCK_DURATION millisecond
	std::chrono::system_clock::time_point m_previousLogRecord;
	std::chrono::system_clock::time_point m_previousLockRelease;

public:
	Logger();
	~Logger();
	void Init(const std::string& logFileLocation);
	void Log(const std::string& msg, LogSeverity severity = LogSeverity::normal, bool asyncLogToFile = true);
	std::string GetCurrentLogFileName();
	void WriteToLogFileRaw(const std::string& text);
	bool IsSpamPreventionEngaged() const;

private:
	void SpamPreventionUpdate();
};