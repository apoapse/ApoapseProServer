#pragma once
#include "LogMessage.h"

struct ILogger
{
	virtual void Log(const LogMessage& logMessage) = 0;
	virtual bool IsSpamPreventionEngaged() const = 0;
};