// ----------------------------------------------------------------------------
// Copyright (C) 2020 Apoapse
// Copyright (C) 2020 Guillaume Puyal
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at http://www.apache.org/licenses/LICENSE-2.0
//
// For more information visit https://github.com/apoapse/
// And https://apoapse.space/
// ----------------------------------------------------------------------------

#pragma once
#include "TypeDefs.hpp"

#define REGISTER_VAR(_var, _defaultVal)	_var = _defaultVal;	serverSettings.RegisterVariable(&_var, #_var);

struct ServerSettingsVars
{
	void RegisterSettingsVariables(class ServerSettings& serverSettings);

	UInt16 server_port;
	bool log_sql_queries;
};
