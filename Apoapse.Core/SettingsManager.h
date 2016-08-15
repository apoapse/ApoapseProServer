#pragma once
#include "ConfigVariable.h"
#include <vector>
#include <boost\property_tree\ptree.hpp>
#include "Diagnostics.h"

class SettingsManager
{
	std::string m_configFilePath;
	boost::property_tree::ptree m_propertyTree;
	std::vector<boost::any> m_registeredConfigs;

public:
	SettingsManager();
	void Init(const std::string&);
	static SettingsManager Create()
	{
		return SettingsManager();
	}

	template <typename U> DLL_API void RegisterConfigVar(const std::string&, const U&);
	template <typename U> DLL_API U ReadConfigValue(const std::string&);
private:
	void LoadConfigFile();
	template <typename U> ConfigVariable<U> GetRegisteredConfigVariableByName(const std::string&);

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