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

#include "stdafx.h"
#include "ServerSettings.h"
#include "Common.h"

#include <boost/property_tree/ini_parser.hpp>

ServerSettings::ServerSettings() : vars(ServerSettingsVars())
{

}

void ServerSettings::Load(const std::string& configFile)
{
	// Reading the registered variables
	vars.RegisterSettingsVariables(*this);
	
	// Parsing the ini file
	boost::property_tree::ptree propertyTree;
	try
	{
		boost::property_tree::ini_parser::read_ini(configFile, propertyTree);
	}
	catch (const std::exception&)
	{
		LOG << LogSeverity::error << "Unable to load or parse the config file " << configFile << ". The default values will the used instead.";
		return;
	}

	// Set the variable to their values defined in the ini config file
	for (auto& registeredVar : m_registeredVariables)
	{
		registeredVar.second->SetParsedValue(propertyTree, registeredVar.first);
	}

	LOG << "Loaded settings from " << configFile;
}
