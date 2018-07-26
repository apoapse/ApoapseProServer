#include "stdafx.h"
#include "ServerSettingsVariables.h"
#include "ServerSettings.h"

void ServerSettingsVars::RegisterSettingsVariables(ServerSettings& serverSettings)
{
	REGISTER_VAR(server_port, 3000);
	//REGISTER_VAR(log_sql_queries, false); // #TODO
}
