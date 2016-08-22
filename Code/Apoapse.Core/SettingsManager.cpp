#include "stdafx.h"
#include "Platforms.h"
#include "SettingsManager.h"
#include "Common.h"
#include <boost\property_tree\json_parser.hpp>
#include <boost\algorithm\string.hpp>

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

template <typename U> void SettingsManager::RegisterConfigVar(const string& configVarName, const U& defaultValue)
{
	boost::lock_guard<boost::mutex> lock(m_mutex);
	m_registeredConfigs.push_back(ConfigVariable<U>(configVarName, defaultValue));
}

template <typename U> U SettingsManager::ReadConfigValue(const string& configVarName)
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
	catch (const boost::property_tree::ptree_error e)
	{
		Log("Can't find the value in the config file. Use the default value from the registered variable instead.", LogSeverity::debug);

		ConfigVariable<U> registedVariable = GetRegisteredConfigVariableByName<U>(configVarName);
		return registedVariable.defaultValue;
	}
}

template <typename U> ConfigVariable<U> SettingsManager::GetRegisteredConfigVariableByName(const std::string& configVarName)
{
	auto itr = std::find_if(m_registeredConfigs.begin(), m_registeredConfigs.end(), [&configVarName](const boost::any& obj)
	{
		try
		{
			ConfigVariable<U> configVar = boost::any_cast<ConfigVariable<U>>(obj);
			return boost::iequals(configVar.name, configVarName);
		}
		catch (std::exception&)	// In case the cast fail, iterate to the next item
		{
			return false;
		}
	});

	if (itr != m_registeredConfigs.end())
	{
		auto index = std::distance(m_registeredConfigs.begin(), itr);

		return boost::any_cast<ConfigVariable<U>>(m_registeredConfigs.at(index));
	}

	ASSERT_MSG(false, "The config variable: " + configVarName + " is not registered");
	throw "The config variable: " + configVarName + " is not registered";
}