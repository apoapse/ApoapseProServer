#pragma once
#include <string>

struct ISettingsManager
{
	virtual void RegisterConfigVar_string(const std::string& configVarName, const std::string& defaultValue) = 0;
	virtual void RegisterConfigVar_int(const std::string& configVarName, const int defaultValue) = 0;
	virtual void RegisterConfigVar_uint(const std::string& configVarName, const unsigned int defaultValue) = 0;
	virtual void RegisterConfigVar_double(const std::string& configVarName, const double defaultValue) = 0;
	virtual void RegisterConfigVar_bool(const std::string& configVarName, const bool defaultValue) = 0;

	virtual std::string ReadConfigValue_string(const std::string& configVarName) = 0;
	virtual int ReadConfigValue_int(const std::string& configVarName) = 0;
	virtual unsigned int ReadConfigValue_uint(const std::string& configVarName) = 0;
	virtual double ReadConfigValue_double(const std::string& configVarName) = 0;
	virtual bool ReadConfigValue_bool(const std::string& configVarName) = 0;
};