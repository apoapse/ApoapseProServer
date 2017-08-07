#pragma once
#include "TypeDefs.hpp"

#define REGISTER_VAR(_var, _defaultVal)	_var = _defaultVal;	serverSettings.RegisterVariable(&_var, #_var);

struct ServerSettingsVars
{
	void RegisterSettingsVariables(class ServerSettings& serverSettings);

	UInt16 server_port;
};
