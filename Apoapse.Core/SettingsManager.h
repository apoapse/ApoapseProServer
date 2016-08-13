#pragma once
#include "ConfigVariable.h"

#ifdef DLL_EXPORT
#define SETTINGS_MANAGER __declspec(dllexport) 
#else
#define SETTINGS_MANAGER __declspec(dllimport) 
#endif

namespace ApoapseCore
{
	class SettingsManager
	{
		std::string m_configFilePath;
		boost::property_tree::ptree m_propertyTree;
		std::vector<boost::any> m_registeredConfigs;

	public:
		SettingsManager();
		SETTINGS_MANAGER void Init(const std::string&);
		SETTINGS_MANAGER static SettingsManager Create()
		{
			return SettingsManager();
		}

		template <typename U> SETTINGS_MANAGER void RegisterConfigVar(const string&, const U&);
		template <typename U> SETTINGS_MANAGER U ReadConfigValue(const std::string&);
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
}