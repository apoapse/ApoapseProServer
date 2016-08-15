#include "stdafx.h"
#include "Common.h"
#include "Logger.h"

Global* global = nullptr;

Global::Global()
{
	logger = new Logger();
	settings = new SettingsManager();
}