#pragma once
#include "ApoapseServer.h"
#include <boost/property_tree/ptree.hpp>
#include <boost/optional.hpp>

enum class Format
{
	JSON,
	INLINE
};

template <typename T>
struct CommmandField
{
	string name;
	bool isRequired;
	boost::optional<std::function<bool(T)>> fieldValueValidator;
};

struct CommandConfig
{
	string name;
	Format format;
	std::vector<CommmandField<string>> fields;
};

class Command
{
	boost::property_tree::ptree m_fields;
	bool m_isValid = { true };

public:
	Command(ApoapseServer& apoapseServer);
	virtual ~Command();
	void FromRawCmd(string& u8cmdText);
	bool IsValid() const;

private:
	void ValidateInternal();

protected:
	ApoapseServer& m_server;
	virtual const CommandConfig& GetConfig() const = 0;
	virtual bool PostValidate() const = 0;
	boost::optional<string> GetFieldValue(const string& fieldName);
};
