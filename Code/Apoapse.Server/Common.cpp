#include "stdafx.h"
#include "Common.h"

Global* global = nullptr;

void FatalError(const string& msg)
{
	LOG << LogSeverity::fatalError << msg;

	std::abort();
}