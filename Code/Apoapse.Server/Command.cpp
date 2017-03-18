#include "stdafx.h"
#include "Command.h"
#include "Common.h"
#include "StringExtensions.h"
#include <boost/property_tree/json_parser.hpp>
#include <iosfwd>
#include "ApoapseError.h"
#include "LocalUser.h"
#include "ClientConnection.h"

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
			string u16CmdText = commandInfoRawBody;
			std::vector<string> tempValues;

			StringExtensions::split(u16CmdText, tempValues, string(" "));

			for (size_t i = 0; i < GetConfig().fields.size(); i++)
			{
				if (i >= tempValues.size())	// Prevent vector overflow in case there are less command arguments than expected
					continue;

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

	auto finalt = m_commandInfoRawBody.get();
}

void Command::AppendPayloadData(const byte* bytesArray, size_t length)
{
	ASSERT(ActualPayloadSize() + length <= ExpectedPayloadSize());	// #TODO Throw and exception and handle it on GenericConnection

	if (ActualPayloadSize() == 0)
		m_payload->reserve(ActualPayloadSize());

	m_payload->insert(m_payload->end(), &bytesArray[0], &bytesArray[length]);
}

void Command::SetPayload(std::shared_ptr<std::vector<byte>> data)
{
	m_payload = data;
}

void Command::AutoValidateInternal()
{
	if (!IsValid())
		return;

	if (GetConfig().payloadExpected && !m_fields.get_optional<string>("payload_size").is_initialized())	//TODO: content length
	{
		LOG << "Command pre-validation (" << GetConfig().name << "), a payload is expected but the field payload_size is missing" << LogSeverity::warning;
		m_isValid = false;
		return;
	}

	for (size_t i = 0; i < GetConfig().fields.size(); i++)
	{
		const auto& currentField = GetConfig().fields.at(i);

		if (!ValidateField(currentField))
		{
			m_isValid = false;

#ifndef DEBUG	// In debug mode we continue the pre-validation process in order to see all potential errors
			return;
#endif
		}
	}

	if (m_isValid && !PostValidate())
		m_isValid = false;
}

bool Command::ValidateField(const CommandField& field)
{
	if (field.requirement == FieldRequirement::ARRAY_MENDATORY)
	{
		const auto values = ReadFieldArray<string>(field.name);

		if (values.size() < 1)
		{
			LOG << "Command pre-validation (" << GetConfig().name << "), required field array " << field.name << " is missing or empty" << LogSeverity::warning;
			return false;
		}

		for (auto& value : values)
		{
			if (!ValidateFieldValue(value, field))
			{
#ifndef DEBUG	// In debug mode we continue the pre-validation process in order to see all potential errors
				return false;
#endif
			}
		}

		return true;
	}
	else
	{
		const auto value = ReadFieldValue<string>(field.name);
		const bool valueExist = value.is_initialized();

		if (field.requirement == FieldRequirement::VALUE_MENDATORY && !valueExist)
		{
			LOG << "Command pre-validation (" << GetConfig().name << "), required field " << field.name << " is missing" << LogSeverity::warning;
			return false;
		}
		
		if (valueExist)
			return ValidateFieldValue(value.get(), field);
		else
			return true;
	}
}

inline bool Command::ValidateFieldValue(const string& value, const CommandField& field)
{
	auto validator = field.fieldValueValidator;

	if (validator.is_initialized() && !validator.get()->ExecValidator(value))
	{
		LOG << "Command pre-validation (" << GetConfig().name << "), field " << field.name << " is not valid" << LogSeverity::warning;
		return false;
	}
	else
		return true;
}

size_t Command::ActualPayloadSize() const
{
	return m_payload->size();
}

bool Command::IsValid() const
{
	return m_isValid;
}

UInt64 Command::ExpectedPayloadSize()
{
	const auto payloadSizeField = ReadFieldValue<UInt64>("payload_size");

	return (GetConfig().payloadExpected && payloadSizeField.is_initialized()) ? payloadSizeField.get() : 0;
}

Format Command::GetInputRealFormat() const
{
	ASSERT(m_inputRealFormat != Format::UNDEFINED);

	return m_inputRealFormat;
}

void Command::SetInputRealFormat(Format format)
{
	ASSERT_MSG(m_inputRealFormat == Format::UNDEFINED, "Input real format already defined");

	m_inputRealFormat = format;
}

void Command::ProcessFromNetwork(ClientConnection* connection)
{
#ifdef DEBUG
	GetConfig().processFromClient(*connection);
#else
	try
	{
		GetConfig().processFromClient(*connection);
	}
	catch (const std::exception& e)
	{
		LOG << e.what() << LogSeverity::error;

		if (connection != nullptr)
		{
			ApoapseError::SendError(ApoapseErrorCode::INTERNAL_SERVER_ERROR, *connection);
			connection->Close();
		}
	}
#endif
}

void Command::ProcessFromNetwork(LocalUser* user, ClientConnection& callingConnection)
{
	bool success;

#ifdef DEBUG
	success = GetConfig().processFromUser(*user, callingConnection);
#else
	try
	{
		success = GetConfig().processFromUser(*user, callingConnection);
	}
	catch (const std::exception& e)
	{
		LOG << e.what() << LogSeverity::error;

		if (user != nullptr)
		{
			ApoapseError::SendError(ApoapseErrorCode::INTERNAL_SERVER_ERROR, *user);
			user->Disconnect();
		}
	}
#endif

	// Propagate the command to the others connections of the user
	if (success && GetConfig().propagateToUser && user->GetConnectionsCount() > 1)
		Send(*user, &callingConnection);
}

void Command::ProcessFromNetwork(RemoteServer* remoteServer)
{
	if (remoteServer == nullptr)
		return;

#ifdef DEBUG
	GetConfig().processFromRemoteServer(*remoteServer);
#else
	try
	{
		GetConfig().processFromRemoteServer(*remoteServer);
	}
	catch (const std::exception& e)
	{
	LOG << e.what() << LogSeverity::error;
		ApoapseError::SendError(ApoapseErrorCode::INTERNAL_SERVER_ERROR, *remoteServer);
	}
#endif
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

void Command::Send(INetworkSender& destination, TCPConnection* excludedConnection /*= nullptr*/, Format forcedOutputFormat /*= Format::UNDEFINED*/)
{
	bool sendPayload = false;
	const Format outputFormat = (forcedOutputFormat == Format::UNDEFINED) ? GetConfig().expectedFormat : forcedOutputFormat;
	ASSERT_MSG(!(GetConfig().payloadExpected && outputFormat == Format::INLINE), "Cannot use an inline format on command with a payload");

	if (GetConfig().payloadExpected && ActualPayloadSize() > 0)
	{
		InsertFieldValue<UInt64>("payload_size", (UInt64)ActualPayloadSize());
		sendPayload = true;
	}

	// In debug and security builds, we check if the system inputs are valid
#if DEBUG || ENABLE_SEC_ADVANCED_CHECKS
	AutoValidateInternal();
	ASSERT(IsValid());
#endif

	std::stringstream outputStream;
	outputStream << GetConfig().name << '\n';

	if (outputFormat == Format::JSON)
	{
		// JSON
		boost::property_tree::write_json(outputStream, m_fields, false);
		outputStream << '\n';	// We add only one line break as boost::property_tree::write_json already add one at the end
	}
	else
	{
		// INLINE
		{
			int i = 0;
			const size_t nbFields = m_fields.size();
			for (auto& field : m_fields)
			{
				i++;
				outputStream << field.second.data();

				if (i < nbFields)
					outputStream << ' ';
			}
		}

		outputStream << '\n';
	}

	destination.Send(std::make_unique<string>(outputStream.str()), excludedConnection);	// Send command body

	if (sendPayload)
		destination.Send(m_payload, excludedConnection);	// Send the payload
}

const std::vector<byte>& Command::GetPayload() const
{
	return *m_payload;
}

std::shared_ptr<std::vector<byte>> Command::GetPayloadPtr() const
{
	return m_payload;
}
