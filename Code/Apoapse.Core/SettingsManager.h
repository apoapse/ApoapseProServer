#pragma once
#include "ConfigVariable.h"
#include <vector>
#include <boost\property_tree\ptree.hpp>
#include "Diagnostics.h"
#include <boost\thread\mutex.hpp>

class SettingsManager
{
	std::string m_configFilePath;
	boost::property_tree::ptree m_propertyTree;
	std::vector<boost::any> m_registeredConfigs;
	boost::mutex m_mutex;

public:
	SettingsManager();
	DLL_API void Init(const std::string& configFilePath);

	template <typename U> DLL_API void RegisterConfigVar(const std::string& configVarName, const U& defaultValue);
	template <typename U> DLL_API U ReadConfigValue(const std::string& configVarName);

	DLL_API static SettingsManager* Create()
	{
		return new SettingsManager();
	}

private:
	void LoadConfigFile();
	template <typename U> ConfigVariable<U> GetRegisteredConfigVariableByName(const std::string& configVarName);

	void ForceExplicitTemplateInstanciation()	// Explicitely instanciate for the compiler all the supported types
	{
		ASSERT_MSG(false, "This function should never be called");

		RegisterConfigVar<std::string>(NULL, NULL);
		RegisterConfigVar<int>(NULL, NULL);
		RegisterConfigVar<unsigned int>(NULL, NULL);
		RegisterConfigVar<bool>(NULL, NULL);

		ReadConfigValue<std::string>(NULL);
		ReadConfigValue<int>(NULL);
		ReadConfigValue<unsigned int>(NULL);
		ReadConfigValue<bool>(NULL);
	}
};