#include "stdafx.h"
#include "Common.h"
#include "Logger.h"
#include <boost\thread.hpp>
#include <fstream>

Logger::Logger() : m_logLock(false), m_timedLogCount(-1)
{
}

void Logger::Init(const string& logFileLocation)
{
	m_logFileLocation = logFileLocation;
	m_previousLogRecord = std::chrono::system_clock::now();

	Log("Logger initialized");
}

Logger::~Logger()
{
}

void Logger::Log(const string& msg, LogSeverity severity, bool asyncLogToFile)
{
#ifdef ENABLE_SPAM_PREVENTION
	SpamPreventionUpdate();
	if (IsSpamPreventionEngaged())
		return;

#endif // ENABLE_SPAM_PREVENTION

	std::shared_ptr<LogMessage> log = std::make_shared<LogMessage>(msg, severity);
	log->LogToConsole();

	//	Log to file
	if (asyncLogToFile)
		global->jobManager->Push([log] { log->LogToFile(); });
	else
		log->LogToFile();
}

string Logger::GetCurrentLogFileName()
{
	return m_logFileLocation;
}

void Logger::WriteToLogFileRaw(const string& text)
{
	boost::lock_guard<boost::mutex> lock(m_mutex);
	//	TODO: test if file is readable
	std::ofstream ofs;
	ofs.open(GetCurrentLogFileName(), std::ofstream::out | std::ofstream::app);

	ofs << text << '\n';

	ofs.close();
}

void Logger::SpamPreventionUpdate()
{
#ifdef ENABLE_SPAM_PREVENTION
	const auto now = std::chrono::system_clock::now();
	const std::chrono::milliseconds previousLogDifference = std::chrono::duration_cast<std::chrono::milliseconds>(now - m_previousLogRecord);
	const std::chrono::milliseconds previousLockReleaseDifference = std::chrono::duration_cast<std::chrono::milliseconds>(now - m_previousLockRelease);

	if ((previousLockReleaseDifference.count() > SPAM_PREVENTION_MAX_LOCK_DURATION) || m_timedLogCount == -1)
	{
		m_timedLogCount = 0;

		if (m_logLock)
		{
			LogMessage warningMsg("Log spam prevention released", LogSeverity::normal);
			warningMsg.LogToConsole();
			warningMsg.LogToFile();
		}

		m_logLock = false;
		m_previousLockRelease = now;
	}
	else
	{
		++m_timedLogCount;

		if (m_timedLogCount >= 3 && previousLogDifference.count() < 150)	// lock if there was a log call less than 150 milliseconds ago but allow 3 logs to be displayed
		{
			if (!m_logLock)
			{
				LogMessage warningMsg("Log spam prevention engaged: too many log calls in a short period of time", LogSeverity::warning);
				warningMsg.LogToConsole();
				warningMsg.LogToFile();

				m_logLock = true;
			}
		}
	}

	m_previousLogRecord = std::chrono::system_clock::now();
#endif // ENABLE_SPAM_PREVENTION
}

bool Logger::IsSpamPreventionEngaged() const
{
	return m_logLock;
}