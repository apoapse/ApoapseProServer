#pragma once
#include <boost/property_tree/ptree.hpp>
#include <boost/optional.hpp>	// #TODO replace with C++17 std
#include <boost/lexical_cast.hpp>
#include "ByteUtils.h"
#include "INetworkSender.h"

class LocalUser;
class RemoteServer;
class ClientConnection;

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
#define PROCESS_METHOD(_inputType, _method)	[this](_inputType& input) { _method(input); };

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
	Format expectedFormat;
	std::vector<CommmandField> fields;
	std::function<void(ClientConnection&)> processFromClient = { NULL };
	std::function<void(LocalUser&, ClientConnection&)> processFromUser = { NULL };
	std::function<void(RemoteServer&)> processFromRemoteServer = { NULL };
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

	void ParseRawCmdBody();
	void AppendCommandBodyData(const string& data);
	bool IsValid() const;
	Format GetInputRealFormat() const;
	void SetInputRealFormat(Format format);

	void ProcessFromNetwork(ClientConnection* connection);
	void ProcessFromNetwork(LocalUser* user, ClientConnection& callingConnection);
	void ProcessFromNetwork(RemoteServer* remoteServer);

	bool CanProcessFrom(ClientConnection*);
	bool CanProcessFrom(LocalUser*);
	bool CanProcessFrom(RemoteServer*);

	void Send(INetworkSender& destination);
	
	template <typename T>
	void InsertFieldValue(const string& path, const T& value)
	{
		m_fields.add(path, value);
	}

	virtual const CommandConfig& GetConfig() = 0;

private:
	void AutoValidateInternal();

	template <typename T>
	inline void InternalCmdProcess(T& input, const std::function<void(T&)>& func)
	{
		ASSERT(func);
		func(input);
	}

protected:
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