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

#pragma once
#include "ServerSettingsVariables.h"
#include <typeindex>
#include <map>
#include <any>
#include <boost/property_tree/ptree.hpp>
#include <boost/optional.hpp>
#include "Common.h"

class ServerSettings
{
	friend ServerSettingsVars;

	struct IConfigVarSingature
	{
		virtual void SetParsedValue(const boost::property_tree::ptree& propertyTree, const std::string& varName) = 0;
	};

	template <typename T>
	class ConfigVarSingature : public IConfigVarSingature
	{
		T* m_variableAddress;

	public:
		ConfigVarSingature(T* var) : m_variableAddress(var)
		{
		}

		void SetParsedValue(const boost::property_tree::ptree& propertyTree, const std::string& varName) override
		{
			boost::optional<T> parsedValueOptional = propertyTree.get_optional<T>(varName);

			if (parsedValueOptional.is_initialized())
				*m_variableAddress = parsedValueOptional.value();
			else
				LOG << "The variable " << varName << " is not set in the config file or is from a wrong type. Using the default value." << LogSeverity::warning;
		}
	};

	std::map<std::string, std::unique_ptr<IConfigVarSingature>> m_registeredVariables;

public:
	ServerSettingsVars vars;
	ServerSettings();

	void Load(const std::string& configFile);

private:
	//void Set
	
private:
	template <typename T>
	void RegisterVariable(T* variable, const std::string& varName)
	{
		static_assert((std::is_same<T, std::string>::value || std::is_same<T, bool>::value || std::is_integral<T>::value), "Server settings: unsupported variable type");

		m_registeredVariables[varName] = std::make_unique<ConfigVarSingature<T>>(variable);
	}

};