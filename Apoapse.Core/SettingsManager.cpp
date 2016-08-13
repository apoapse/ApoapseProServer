#include "stdafx.h"
#include "SettingsManager.h"

using namespace ApoapseCore;

SettingsManager::SettingsManager()
{
	
}

void SettingsManager::Init(const string& configFilePath)
{
	m_configFilePath = configFilePath;

	LoadConfigFile();
}

void SettingsManager::LoadConfigFile()
{
	try
	{
		boost::property_tree::read_json(m_configFilePath, m_propertyTree);
	}
	catch (const std::exception& e)
	{
		//TODO: Handle excepetion -> fatal error
		printf(e.what());
	}
}

template <typename U> void SettingsManager::RegisterConfigVar(const string& configVarName, const U& defaultValue)
{
	//TODO: assert
	m_registeredConfigs.push_back(ConfigVariable<U>(configVarName, defaultValue));
}

template <typename U> U SettingsManager::ReadConfigValue(const string& configVarName)
{
#ifdef DEBUG
	//	In debug mode, check if the varible is registered even if it is already implemented in the config file
	GetRegisteredConfigVariableByName<U>(configVarName);
#endif

	try
	{
		return m_propertyTree.get<U>(configVarName);
	}
	catch (const boost::property_tree::ptree_error e)
	{
		//TODO: Debug LOG

		//	Can't find the value in the config file. Use the default value from the registered variable instead.
		ConfigVariable<U> registedVariable = GetRegisteredConfigVariableByName<U>(configVarName);
		return registedVariable.defaultValue;
	}
}

template <typename U> ConfigVariable<U> SettingsManager::GetRegisteredConfigVariableByName(const std::string& configVarName)
{
	auto itr = std::find_if(m_registeredConfigs.begin(), m_registeredConfigs.end(), [&configVarName](const boost::any& obj)
	{
		ConfigVariable<U> configVar = boost::any_cast<ConfigVariable<U>>(obj);
		return boost::iequals(configVar.name, configVarName);
	});

	if (itr != m_registeredConfigs.end())
	{
		auto index = std::distance(m_registeredConfigs.begin(), itr);

		return boost::any_cast<ConfigVariable<U>>(m_registeredConfigs.at(index));
	}

	ASSERT_MSG(false, "The config variable: " << configVarName << " is not registered");
	throw "Unregistered config variable";
}