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
			string u16CmdText = commandInfoRawBody;	// #TODO #IMPORTANT Convert to UTF-16?
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
		m_payload.reserve(ActualPayloadSize());

	m_payload.insert(m_payload.end(), &bytesArray[0], &bytesArray[length]);
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

			// In debug mode we continue the pre-validation process in order to see all potential errors
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

size_t Command::ActualPayloadSize() const
{
	return m_payload.size();
}

bool Command::IsValid() const
{
	return m_isValid;
}

UInt64 Command::ExpectedPayloadSize()
{
	const auto payloadSizeField = ReadFieldValue<UInt64>("payload_size");

	return (GetConfig().isPayloadExpected && payloadSizeField.is_initialized()) ? payloadSizeField.get() : 0;
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
	if (connection == nullptr)
		return;

	try
	{
		GetConfig().processFromClient(*connection);
	}
	catch (const std::exception& e)
	{
		LOG << e.what() << LogSeverity::error;
		ApoapseError::SendError(ApoapseErrorCode::INTERNAL_SERVER_ERROR, *connection);
	}
}

void Command::ProcessFromNetwork(LocalUser* user, ClientConnection& callingConnection)
{
	if (user == nullptr)
		return;

	try
	{
		GetConfig().processFromUser(*user, callingConnection);
	}
	catch (const std::exception& e)
	{
		LOG << e.what() << LogSeverity::error;
		ApoapseError::SendError(ApoapseErrorCode::INTERNAL_SERVER_ERROR, *user);
	}
}

void Command::ProcessFromNetwork(RemoteServer* remoteServer/*, ClientConnection& callingConnection*/) // #TODO 
{
	if (remoteServer == nullptr)
		return;

	throw std::logic_error("TODO IMPLEMENT");
// 	try
// 	{
// 		GetConfig().processFromRemoteServer(*remoteServer);
// 	}
// 	catch (const std::exception& e)
// 	{
// 	LOG << e.what() << LogSeverity::error;
// 		ApoapseError::SendError(ApoapseErrorCode::INTERNAL_SERVER_ERROR, *remoteServer);
// 	}
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

void Command::Send(INetworkSender& destination, Format forcedOutputFormat/* = Format::UNDEFINED*/)
{
	// In debug and security builds, we check if the system inputs are valid
#if DEBUG || ENABLE_SEC_ADVANCED_CHECKS
	AutoValidateInternal();
	ASSERT(IsValid());
#endif

	const Format outputFormat = (forcedOutputFormat == Format::UNDEFINED) ? GetConfig().expectedFormat : forcedOutputFormat;
	ASSERT_MSG(!(GetConfig().isPayloadExpected && outputFormat == Format::INLINE), "Cannot use an inline format on command with a payload");

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

	destination.Send(outputStream.str());	// Send command body

	if (outputFormat == Format::JSON && ActualPayloadSize() > 0)
		destination.Send(m_payload);	// Send payload
}

const std::vector<byte>& Command::GetPayload() const
{
	return m_payload;
}
