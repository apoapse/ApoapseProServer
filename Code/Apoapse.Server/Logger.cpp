#include "stdafx.h"
#include "Common.h"
#include "Logger.h"
#include <boost\thread.hpp>
#include <fstream>

Logger::Logger() : m_logLock(false), m_lockedLogCount(0)
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
	std::ofstream ofs;
	ofs.open(GetCurrentLogFileName(), std::ofstream::out | std::ofstream::app);

	if (!ofs.is_open())
		return;

	ofs << text << '\n';

	ofs.close();
}

void Logger::SpamPreventionUpdate()
{
#ifdef ENABLE_SPAM_PREVENTION

	const UInt32 maxAllowedConsecutiveLogs = MAX_ALLOWED_CONSECUTIVE_LOGS;

	//const auto now = std::chrono::system_clock::now();
	const auto now = std::chrono::steady_clock::now();
	const std::chrono::milliseconds previousLogDifference = std::chrono::duration_cast<std::chrono::milliseconds>(now - m_previousLogRecord);
	++m_lockedLogCount;

	if (previousLogDifference.count() <= 160 && m_lockedLogCount >= maxAllowedConsecutiveLogs)
	{
		if (!m_logLock)
		{
			LogMessage warningMsg("Log spam prevention engaged: too many log calls in a short period of time", LogSeverity::warning);
			warningMsg.LogToConsole();
			warningMsg.LogToFile();

			m_logLock = true;
		}
	}
	else
	{
		if (m_logLock)
		{
			LogMessage warningMsg(Format("Log spam prevention released (approximately %1% log calls blocked)", (m_lockedLogCount - maxAllowedConsecutiveLogs)), LogSeverity::normal);
			warningMsg.LogToConsole();
			warningMsg.LogToFile();

			m_logLock = false;
			m_lockedLogCount = 0;
		}
	}

	m_previousLogRecord = now;

#endif // ENABLE_SPAM_PREVENTION
}

bool Logger::IsSpamPreventionEngaged() const
{
	return m_logLock;
}