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
	}

	// Set the variable to their values defined in the ini config file
	for (auto& registeredVar : m_registeredVariables)
	{
		registeredVar.second->SetParsedValue(propertyTree, registeredVar.first);
	}
}
