#pragma once
#include "ISettingsManager.h"
#include "ConfigVariable.h"
#include <map>
#include <boost\property_tree\ptree.hpp>
#include "Diagnostics.h"
#include <boost\thread\mutex.hpp>

class SettingsManager : public ISettingsManager
{
	std::string m_configFilePath;
	boost::property_tree::ptree m_propertyTree;
	std::map<std::string, boost::any> m_registeredConfigs;	// <config var name, default value>
	boost::mutex m_mutex;

public:
	SettingsManager();
	DLL_API void Init(const std::string& configFilePath);

	DLL_API void RegisterConfigVar_string(const std::string& configVarName, const std::string& defaultValue);
	DLL_API void RegisterConfigVar_int(const std::string& configVarName, const int defaultValue);
	DLL_API void RegisterConfigVar_uint(const std::string& configVarName, const unsigned int defaultValue);
	DLL_API void RegisterConfigVar_double(const std::string& configVarName, const double defaultValue);
	DLL_API void RegisterConfigVar_bool(const std::string& configVarName, const bool defaultValue);

	DLL_API std::string ReadConfigValue_string(const std::string& configVarName);
	DLL_API int ReadConfigValue_int(const std::string& configVarName);
	DLL_API unsigned int ReadConfigValue_uint(const std::string& configVarName);
	DLL_API double ReadConfigValue_double(const std::string& configVarName);
	DLL_API bool ReadConfigValue_bool(const std::string& configVarName);

	DLL_API static SettingsManager* Create()
	{
		return new SettingsManager();
	}

private:
	void LoadConfigFile();
	template <typename U> ConfigVariable<U> GetRegisteredConfigVariableByName(const std::string& configVarName);
	template <typename U> void InternalRegisterConfigVar(const std::string& configVarName, const U& defaultValue);
	template <typename U> U InternalReadConfigValue(const std::string& configVarName);
};