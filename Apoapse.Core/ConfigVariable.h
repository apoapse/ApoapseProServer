#pragma once
#include <string>

template <class T> struct ConfigVariable
{
	ConfigVariable(const std::string& name, const T& defaultValue) : name(name), defaultValue(defaultValue)
	{
	}

	const std::string name;
	const T defaultValue;

	bool operator== (const ConfigVariable& obj) const
	{
		return boost::iequals(obj.name, this->name);
	}
};