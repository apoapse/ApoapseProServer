#include "stdafx.h"
#include "Platforms.h"
#include "SettingsManager.h"
#include "Common.h"
#include <boost\property_tree\json_parser.hpp>
#include <boost\algorithm\string.hpp>
#include <boost\thread.hpp>

SettingsManager::SettingsManager()
{
}

void SettingsManager::Init(const string& configFilePath)
{
	m_configFilePath = configFilePath;

	LoadConfigFile();

	Log("SettingsManager initialized");
}

void SettingsManager::LoadConfigFile()
{
	boost::property_tree::read_json(m_configFilePath, m_propertyTree);

	ASSERT(m_propertyTree.size() > 0);
}

//////////////////////////////////////////////////////////////
//	REGISTER
//////////////////////////////////////////////////////////////
template <typename U> void SettingsManager::InternalRegisterConfigVar(const string& configVarName, const U& defaultValue)
{
	boost::lock_guard<boost::mutex> lock(m_mutex);

	if (!m_registeredConfigs.insert(std::make_pair(configVarName, defaultValue)).second)
	{
		Log(Format("%1% config variable %2% is already registered", __FUNCTION__, configVarName), LogSeverity::warning);
	}
}

void SettingsManager::RegisterConfigVar_string(const std::string& configVarName, const string& defaultValue)
{
	InternalRegisterConfigVar<string>(configVarName, defaultValue);
}

void SettingsManager::RegisterConfigVar_int(const std::string& configVarName, const int defaultValue)
{
	InternalRegisterConfigVar<int>(configVarName, defaultValue);
}

void SettingsManager::RegisterConfigVar_uint(const std::string& configVarName, const unsigned int defaultValue)
{
	InternalRegisterConfigVar<unsigned int>(configVarName, defaultValue);
}

void SettingsManager::RegisterConfigVar_double(const std::string& configVarName, const double defaultValue)
{
	InternalRegisterConfigVar<double>(configVarName, defaultValue);
}

void SettingsManager::RegisterConfigVar_bool(const std::string& configVarName, const bool defaultValue)
{
	InternalRegisterConfigVar<bool>(configVarName, defaultValue);
}


//////////////////////////////////////////////////////////////
//	READ
//////////////////////////////////////////////////////////////
template <typename U> U SettingsManager::InternalReadConfigValue(const string& configVarName)
{
	boost::lock_guard<boost::mutex> lock(m_mutex);

#ifdef DEBUG
	//	In debug mode, we check if the variable is registered even if it is already implemented in the config file
	GetRegisteredConfigVariableByName<U>(configVarName);
#endif

	try
	{
		return m_propertyTree.get<U>(configVarName);	// #TODO What happen if the value in the config file is not of the right type?
	}
	catch (const boost::property_tree::ptree_error e)	// #TODO find a better way that do not use exceptions
	{
		Log("Can't find the value in the config file. Use the default value from the registered variable instead.", LogSeverity::debug);

		ConfigVariable<U> registedVariable = GetRegisteredConfigVariableByName<U>(configVarName);
		return registedVariable.defaultValue;
	}
}

template <typename U> ConfigVariable<U> SettingsManager::GetRegisteredConfigVariableByName(const std::string& configVarName)
{
	auto itr = m_registeredConfigs.find(configVarName);

	if (itr != m_registeredConfigs.end())
	{
		auto test = itr->second;
		U defaultValue = boost::any_cast<U>(itr->second);	// #TODO: what happen if the conversion fail?

		return ConfigVariable<U>(configVarName, defaultValue);
	}
	else
	{
		ASSERT_MSG(false, "The config variable is not registered");
		throw "The config variable: " + configVarName + " is not registered";
	}
}

string SettingsManager::ReadConfigValue_string(const string& configVarName)
{
	return InternalReadConfigValue<string>(configVarName);
}

int SettingsManager::ReadConfigValue_int(const string& configVarName)
{
	return InternalReadConfigValue<int>(configVarName);
}

unsigned int SettingsManager::ReadConfigValue_uint(const string& configVarName)
{
	return InternalReadConfigValue<unsigned int>(configVarName);
}

double SettingsManager::ReadConfigValue_double(const string& configVarName)
{
	return InternalReadConfigValue<double>(configVarName);
}

bool SettingsManager::ReadConfigValue_bool(const string& configVarName)
{
	return InternalReadConfigValue<bool>(configVarName);
}