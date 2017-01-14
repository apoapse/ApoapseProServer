#include "stdafx.h"
#include "Command.h"
#include "Common.h"
#include "StringExtensions.h"
#include <boost/property_tree/json_parser.hpp>

Command::Command()
{
	LOG << "Command created";
}

Command::~Command()
{
	LOG_DEBUG_FUNCTION_NAME();
}

void Command::ParseRawCmdBody()
{
	auto& commandInfoRawBody = m_commandInfoRawBody.get();

	StringExtensions::trim_left(commandInfoRawBody);
	StringExtensions::trim_right(commandInfoRawBody);

	if (GetInputRealFormat() == Format::JSON)
	{
		// is JSON
		std::stringstream ss;
		ss << std::move(commandInfoRawBody);

		try
		{
			boost::property_tree::read_json(ss, m_fields);
		}
		catch (const std::exception& e)
		{
			LOG << "Command " << GetConfig().name << ": error reading json (" << e.what() << ")" << LogSeverity::warning;
			m_isValid = false;
			return;
		}
	}
	else
	{
		// is INLINE
		if (GetConfig().expectedFormat == Format::JSON)
		{
			LOG << "Command " << GetConfig().name << ": expected a json command but an inline was provided" << LogSeverity::warning;
			m_isValid = false;
			return;
		}

		{
			string u16CmdText = commandInfoRawBody;	// #TODO #IMPORTANT Convert to UTF-16
			std::vector<string> tempValues;

			StringExtensions::split(u16CmdText, tempValues, string(" "));

			for (size_t i = 0; i < GetConfig().fields.size(); i++)
			{
				if (i >= tempValues.size())	// Prevent vector overflow in case there are less command arguments than expected
					return;

				m_fields.add(GetConfig().fields.at(i).name, std::move(tempValues.at(i)));
			}
		}
	}

	m_isCommandParsed = true;
	m_commandInfoRawBody.reset();

	AutoValidateInternal();
}

void Command::AppendCommandBodyData(const string& data)
{
	if (m_isCommandParsed)
		ASSERT_MSG(false, "The command as already been parsed");	// #TODO Throw and exception and handle it on GenericConnection

	// #TODO Monitor command body size to make sure it can't be too large

	if (!m_commandInfoRawBody)
		m_commandInfoRawBody = data;
	else
		m_commandInfoRawBody->append(data);
}

void Command::AutoValidateInternal()
{
	if (!IsValid())
		return;

	if (GetConfig().isPayloadExpected && !m_fields.get_optional<string>("payload_size").is_initialized())	//TODO: content length
	{
		LOG << "Command pre-validation (" << GetConfig().name << "), a payload is expected but the field payload_size is missing" << LogSeverity::warning;
		m_isValid = false;
		return;
	}

	for (size_t i = 0; i < GetConfig().fields.size(); i++)
	{
		const auto& currentField = GetConfig().fields.at(i);
		const auto currentVal = m_fields.get_optional<string>(currentField.name);
		const bool isValueExist = currentVal.is_initialized();

		if (currentField.isRequired && !isValueExist)
		{
			LOG << "Command pre-validation (" << GetConfig().name << "), required field " << currentField.name << " is missing" << LogSeverity::warning;
			m_isValid = false;

			// In debug mode we continue the pre-validation process in order to see all the errors
#ifndef DEBUG
			return;
#endif
		}

		if (isValueExist && currentField.IsValidatorInitialized())
		{
			auto validator = currentField.fieldValueValidator;

			if (!validator.get()->ExecValidator(currentVal.get()))
			{
				LOG << "Command pre-validation (" << GetConfig().name << "), field " << currentField.name << " is not valid" << LogSeverity::warning;
				m_isValid = false;

#ifndef DEBUG
				return;
#endif
			}
		}
	}

	if (m_isValid && !PostValidate())
		m_isValid = false;
}

bool Command::IsValid() const
{
	return m_isValid;
}

Format Command::GetInputRealFormat() const
{
	return m_inputRealFormat;
}

void Command::SetInputRealFormat(Format format)
{
	ASSERT_MSG(m_inputRealFormat == Format::UNDEFINED, "Input real format already defined");

	m_inputRealFormat = format;
}

void Command::ProcessFromNetwork(ClientConnection* connection)
{
	if (connection == nullptr)
		return;

	ASSERT(CanProcessFrom(connection));
	InternalCmdProcess(*connection, GetConfig().processFromClient);
}

void Command::ProcessFromNetwork(LocalUser* user)
{
	if (user == nullptr)
		return;

	ASSERT(CanProcessFrom(user));
	InternalCmdProcess(*user, GetConfig().processFromUser);
}

void Command::ProcessFromNetwork(RemoteServer* remoteServer)
{
	if (remoteServer == nullptr)
		return;

	ASSERT(CanProcessFrom(remoteServer));
	InternalCmdProcess(*remoteServer, GetConfig().processFromRemoteServer);
}

bool Command::CanProcessFrom(ClientConnection*)
{
	return GetConfig().processFromClient != NULL;
}

bool Command::CanProcessFrom(LocalUser*)
{
	return GetConfig().processFromUser != NULL;
}

bool Command::CanProcessFrom(RemoteServer*)
{
	return GetConfig().processFromRemoteServer != NULL;
}
