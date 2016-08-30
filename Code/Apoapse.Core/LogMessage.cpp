#include "stdafx.h"
#include "Common.h"
#include "ILogger.h"
#include "LogMessage.h"
#include <iostream>
#include <boost\date_time.hpp>
#ifdef WINDOWS
#include <WinBase.h>
#endif

LogMessage::LogMessage(const string& msg, LogSeverity severity) : message(msg), logSeverity(severity)
{
	m_logFormatedDateTime = GenerateLocalDateTime();
}

void LogMessage::LogToConsole() const
{
#ifdef DEBUG
	Trace();
#endif

#ifdef WINDOWS
	auto color = GetConsoleColorBySeverity(logSeverity);

	if (color != 7)	//	Only if it's not to apply the default color again
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);

	std::cout << message << std::endl;

	if (color != 7)
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);	//	Reset to default color

#else
	PLATFORM_NOT_IMPLEMENTED("unix console colors");
	std::cout << msg << std::endl;
#endif
}

void LogMessage::LogToFile() const
{
	string finalMsg = m_logFormatedDateTime + " - " + GetSeverityPrefix() + message;

	ASSERT(global->logger);
	global->logger->WriteToLogFileRaw(std::ref(finalMsg));
}

#ifdef WINDOWS
WORD LogMessage::GetConsoleColorBySeverity(LogSeverity severity) const
{
	switch (severity)
	{
	case LogSeverity::debug:
		return FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_INTENSITY;	//	Cyan

	case LogSeverity::warning:
		return 14;															//	Yellow

	case LogSeverity::error:
	case LogSeverity::fatalError:
		return FOREGROUND_RED | FOREGROUND_INTENSITY;						// Red

	default:
		return 7;															//	Default color (light gray)
	}
}

#else
string LogMessage::GetColorBySeverity(LogSeverity severity) const
{
	switch (severity)
	{
	case Logger::normal:
		return "\039";	//	Default color

	case Logger::debug:
		return "\036";	//	Cyan
	}
}
#endif

string LogMessage::GetSeverityPrefix() const
{
	switch (logSeverity)
	{
	case LogSeverity::debug:
		return "DEBUG: ";

	case LogSeverity::warning:
		return "WARNING: ";

	case LogSeverity::error:
		return "ERROR: ";

	case LogSeverity::fatalError:
		return "FATAL ERROR: ";
	}

	return "";
}

string LogMessage::GenerateLocalDateTime() const
{
	using boost::posix_time::ptime;
	using namespace boost::gregorian;

	std::stringstream stream;

	ptime currentDate = boost::posix_time::second_clock::local_time();
	date date(currentDate.date());

	date_facet* facet(new date_facet("%a, %d %b %Y "));
	stream.imbue(std::locale(std::locale::classic(), facet));
	stream << date << currentDate.time_of_day();

	return stream.str();
}

void LogMessage::Trace() const
{
#ifdef WINDOWS
	std::wostringstream os_;
	os_ << GetSeverityPrefix().c_str() << message.c_str() << '\n';
	OutputDebugStringW(os_.str().c_str());
#endif
}