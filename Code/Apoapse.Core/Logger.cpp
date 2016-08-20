#include "stdafx.h"
#include "Common.h"
#include "Logger.h"
#include <boost\thread.hpp>
#include <fstream>

Logger::Logger()
{
}

void Logger::Init(const string& logFileLocation)
{
	m_logFileLocation = logFileLocation;

	Log("Logger initialized");
}

Logger::~Logger()
{
}

void Logger::Log(const string& msg, LogSeverity severity, bool asyncLogToFile)
{
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