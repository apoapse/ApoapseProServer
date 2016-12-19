#pragma once
#include <boost/property_tree/ptree.hpp>
#include <boost/optional.hpp>	// #TODO replace with C++17 std
#include <boost/lexical_cast.hpp>
#include "ByteUtils.h"

class LocalUser;
class RemoteServer;

enum class Format
{
	UNDEFINED,
	JSON,
	INLINE
};

struct IFieldValidator
{
	virtual bool ExecValidator(const string& value) const = 0;
};

template <typename T>
class FieldValueValidator : public IFieldValidator
{
	const std::function<bool(T)> m_validatorFunction;

public:
	FieldValueValidator(std::function<bool(T)> validator) : m_validatorFunction(validator)
	{
	}

	bool ExecValidator(const string& value) const override
	{
		try
		{
			T convertedValue = ConvertFromStr(value);

			return m_validatorFunction(convertedValue);
		}
		catch (const std::exception& e)
		{
			LOG << "FieldValueValidator: value type conversion failed (" << e.what() << ") returning false" << LogSeverity::debug;

			return false;
		}
	}

private:
	static T ConvertFromStr(const string& str)
	{
		return boost::lexical_cast<T>(str);
	}
};
#define FIELD_VALUE_VALIDATOR(_type, _func)	new FieldValueValidator<_type>(_func)

struct CommmandField
{
	string name;
	bool isRequired;
	boost::optional<IFieldValidator*> fieldValueValidator;

	bool IsValidatorInitialized() const
	{
		return fieldValueValidator.is_initialized();
	}
};

struct CommandConfig
{
	string name;
	Format format;
	std::vector<CommmandField> fields;
	//boost::optional<std::function<void(LocalUser)>> fieldValueValidator;	// Integrate as functions (maybe overloaded functions?) in this very class and in these, check if the vars are defined and if they are, call them
	//boost::optional<std::function<void(RemoteServer)>> fieldValueValidator;
	//boost::optional<std::function<void(GenericConnection)>> fieldValueValidator;
	bool isPayloadExpected = { false };
};

class Command
{
	boost::property_tree::ptree m_fields;
	bool m_isValid = { true };
	Format m_inputRealFormat = { Format::UNDEFINED };
	boost::optional<std::vector<byte>> m_payload;	//TODO
	boost::optional<string> m_commandInfoRawBody;
	bool m_isCommandParsed = { false };

public:
	//static const Int16 COMMAND_NAME_MAX_SIZE = 255;	// #TODO

	Command();
	virtual ~Command();

	void ParseRawCmdBody();
	void FromRawCmd(string& u8cmdText);
	void AppendCommandBodyData(const string& data);
	bool IsValid() const;
	Format GetInputRealFormat() const;
	void SetInputRealFormat(Format format);

	virtual const CommandConfig& GetConfig() const = 0;

private:
	void ValidateInternal();

protected:
	//ApoapseServer& m_server;

	//************************************
	// Method:    Connect::PostValidate - Used to do to additional validations on the fields - called only if the previous automatic validation steps succeeded
	// Access:    public 
	// Returns:   bool
	//************************************
	virtual bool PostValidate() const = 0;

	template <typename T>
	boost::optional<T> ReadFieldValue(const string& fieldName)
	{
		return m_fields.get_optional<T>(fieldName);
	}
};