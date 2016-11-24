#pragma once
#include "eLogSeverity.h"

struct LogMessage
{
	const string msg;
	const LogSeverity logSeverity;
	string severityPrefix;
	string dateTime;

	LogMessage(const string& message, LogSeverity severity) : msg(message), logSeverity(severity)
	{
		GenerateDateTime();	// The time is recorded as soon as possible in order to not loose accuracy once the message is registered in the log file via an async call
		GenerateSeverityPrefix();
	}

private:
	void GenerateSeverityPrefix();
	void GenerateDateTime();
};