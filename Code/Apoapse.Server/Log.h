#pragma once
#include <sstream>
#include "Logger.h"

class Log
{
	std::stringstream m_stream;
	Logger& m_logger;
	LogSeverity m_logSeverity { LogSeverity::normal };

public:
	Log(Logger& logger) : m_logger(logger)
	{
	}

	virtual ~Log()
	{
		m_logger.Log(LogMessage(m_stream.str(), m_logSeverity));
	}

	template<typename T>
	Log& operator<<(const T& item)
	{
		m_stream << item;
		return *this;
	}

	Log& operator<<(LogSeverity logSeverity)
	{
		m_logSeverity = logSeverity;
		return *this;
	}
};