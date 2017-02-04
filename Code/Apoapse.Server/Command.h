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
#define FIELD_VALUE_VALIDATOR(_type, _func)		new FieldValueValidator<_type>(_func)
#define FIELD_VALUE_CHECK_TYPE(_type)			new FieldValueValidator<_type>([](_type){ return true; })
#define PROCESS_METHOD(_inputType, _method)		[this](_inputType& input) { _method(input); };

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

	//************************************
	// Method:    Command::AppendCommandBodyData - Each each call append a part of a raw command before been parsed by calling ParseRawCmdBody
	// Access:    public 
	// Returns:   void
	// Parameter: const string & data
	//************************************
	void AppendCommandBodyData(const string& data);

	bool IsValid() const;

	//************************************
	// Method:    Command::GetInputRealFormat - In opposition to GetConfig().expectedFormat, this method return the actual format of a raw command which has been polulated by AppendCommandBodyData
	// Access:    public 
	// Returns:   Format
	//************************************
	Format GetInputRealFormat() const;
	void SetInputRealFormat(Format format);

	void ProcessFromNetwork(ClientConnection* connection);
	void ProcessFromNetwork(LocalUser* user, ClientConnection& callingConnection);
	void ProcessFromNetwork(RemoteServer* remoteServer);

	bool CanProcessFrom(ClientConnection*);
	bool CanProcessFrom(LocalUser*);
	bool CanProcessFrom(RemoteServer*);

	//************************************
	// Method:    Command::Send - Send the full command to specified destination populated by the values provied by InsertFieldValue or ParseRawCmdBody and use GetConfig().expectedFormat as format
	// Access:    public 
	// Returns:   void
	// Parameter: INetworkSender & destination
	// Parameter: Format forceOutputFormat - 
	//************************************
	void Send(INetworkSender& destination, Format forcedOutputFormat = Format::UNDEFINED);
	
	template <typename T>
	void InsertFieldValue(const string& path, const T& value)
	{
		m_fields.add(path, value);
	}

	template <typename T>
	boost::optional<T> ReadFieldValue(const string& fieldName)
	{
		return m_fields.get_optional<T>(fieldName);
	}

	virtual const CommandConfig& GetConfig() = 0;

private:
	//************************************
	// Method:    Command::AutoValidateInternal - This is executed automaticly after a command has been parsed and use the FieldValueValidator defined in the command config to check if the fields are valid or not 
	// Access:    private 
	// Returns:   void
	//************************************
	void AutoValidateInternal();

	template <typename T>
	inline void InternalCmdProcess(T& inputConnection, const std::function<void(T&)>& func)
	{
		try
		{
			ASSERT(func);
			func(inputConnection);
		}
		catch (const std::exception&)
		{
			ApoapseError::SendError(ApoapseErrorCode::INTERNAL_SERVER_ERROR, connection);
		}
	}

protected:
	//************************************
	// Method:    Connect::PostValidate - Used to do to additional validations on the fields - called only if the previous automatic validation steps succeeded. No I/O operations should be done at this point.
	// Access:    public 
	// Returns:   bool
	//************************************
	virtual bool PostValidate() const = 0;
};