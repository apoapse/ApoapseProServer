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
	LOG << m_commandInfoRawBody;

	m_isCommandParsed = true;
}

void Command::FromRawCmd(string& u8cmdText)
{
	StringExtensions::trim_left(u8cmdText);
	StringExtensions::trim_right(u8cmdText);

	if (u8cmdText.substr(0, 1) == "{" && u8cmdText.substr(u8cmdText.length() - 1, u8cmdText.length()) == "}")
	{
		// is JSON
		std::stringstream ss;
		ss << std::move(u8cmdText);

		try
		{
			boost::property_tree::read_json(ss, m_fields);
			m_inputRealFormat = Format::JSON;//REMOVE
		}
		catch (const std::exception& e)
		{
			LOG << "Command " << GetConfig().name << ": error reading json (" << e.what() << ")" << LogSeverity::warning;
			m_isValid = false;
		}
	}
	else
	{
		// is INLINE
		if (GetConfig().format == Format::JSON)
		{
			LOG << "Command " << GetConfig().name << ": expected a json command but an inline was provided" << LogSeverity::warning;
			m_isValid = false;
			return;
		}

		m_inputRealFormat = Format::INLINE;//REMOVE

		{
			string u16CmdText = u8cmdText;	// #TODO #IMPORTANT Convert to UTF-16
			std::vector<string> tempValues;

			StringExtensions::split(u16CmdText, tempValues, string(" "));

			for (size_t i = 0; i < GetConfig().fields.size(); i++)
			{
				m_fields.add(GetConfig().fields.at(i).name, std::move(tempValues.at(i)));
			}
		}
	}

	//#OPTIMIZATION make sure u8cmdText is free from the memory

	ValidateInternal();	// #TODO #SHORT_TERM Call it via the thread pool? In this case, make sure Command::IsValid is thread safe -> do it from the call of FromRawCmd in TCP Connection?
}

void Command::AppendCommandBodyData(const string& data)
{
	if (m_isCommandParsed)
		ASSERT_MSG(false, "The command as already been parsed");	// #TODO Throw and exception and handle it on GenericConnection

	if (!m_commandInfoRawBody)
		m_commandInfoRawBody = data;
	else
		m_commandInfoRawBody->append(data);

	string test = m_commandInfoRawBody.get();
}

void Command::ValidateInternal()
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
	m_inputRealFormat = format;
}