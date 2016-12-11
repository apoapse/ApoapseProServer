#pragma once
#include "ApoapseServer.h"
#include <boost/property_tree/ptree.hpp>
#include <boost/optional.hpp>	// #TODO replace with C++17 std
#include <boost/any.hpp>
#include "ByteUtils.h"

class LocalUser;
class RemoteServer;

enum class Format
{
	UNDEFINED,
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
	bool isPayloadExpected = { false };
	//boost::optional<std::function<void(LocalUser)>> fieldValueValidator;	// Integrate as functions (maybe overloaded functions?) in this very class and in these, check if the vars are defined and if they are, call them
	//boost::optional<std::function<void(RemoteServer)>> fieldValueValidator;
	//boost::optional<std::function<void(TCPConnection)>> fieldValueValidator;
	std::vector<CommmandField<string>> fields;
};

class Command
{
	boost::property_tree::ptree m_fields;
	bool m_isValid = { true };
	Format m_inputRealFormat = { Format::UNDEFINED };
	boost::optional<std::vector<byte>> m_payload;	//TODO

public:
	//static const Int16 commandNameAllowedMaxSize = 255;	// #TODO

	Command(ApoapseServer& apoapseServer);
	virtual ~Command();
	void FromRawCmd(string& u8cmdText);
	bool IsValid() const;
	Format GetInputRealFormat() const;
	static string ReadCommandNameFromRaw(const string& rawcmdText);

private:
	void ValidateInternal();

protected:
	ApoapseServer& m_server;
	virtual const CommandConfig& GetConfig() const = 0;

	//************************************
	// Method:    Connect::PostValidate - Used to do to additional validations on the fields - called only if the previous automatic validation steps are successful
	// Access:    public 
	// Returns:   bool
	//************************************
	virtual bool PostValidate() const = 0;

	boost::optional<string> ReadFieldValue(const string& fieldName);

	//template <typename T>
	//boost::optional<T> ReadFieldValue(const string& fieldName)
	//{
	//	static_assert(std::is_same<T, bool>::value || std::is_same<T, string>::value, "Unsupported type");
	//}
};
