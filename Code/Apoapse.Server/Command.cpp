#include "stdafx.h"
#include "Command.h"
#include "Common.h"
#include "StringExtensions.h"
#include <boost/property_tree/json_parser.hpp>

Command::Command(ApoapseServer& apoapseServer) : m_server(apoapseServer)
{
	LOG << "Command created";
}

Command::~Command()
{

}

void Command::FromRawCmd(string& u8cmdText)
{
	ASSERT_MSG(u8cmdText.substr(0, GetConfig().name.length()) == GetConfig().name, "The command name is the command raw text does not mach the command name defined in the class");

	if (u8cmdText.length() < GetConfig().name.length())
		ASSERT(false);	//TODO

	u8cmdText = u8cmdText.erase(0, GetConfig().name.length());

	StringExtensions::trim_left(u8cmdText);
	StringExtensions::trim_right(u8cmdText);

	StringExtensions::erase_left(u8cmdText, '{');	// Erase the command name

	if (u8cmdText.substr(0, 1) == "{" && u8cmdText.substr(u8cmdText.length() - 1, u8cmdText.length()) == "}")
	{
		// is JSON
		std::stringstream ss;
		ss << u8cmdText;	// #OPTIMIZATION avoid full copy of the string in the stream

		try
		{
			boost::property_tree::read_json(ss, m_fields);
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

		{
			string u16CmdText = u8cmdText;	// #TODO #IMPORTANT Convert to UTF-16
			std::vector<string> tempValues;

			StringExtensions::split(u16CmdText, tempValues, string(" "));

			const CommandConfig localConfig = GetConfig();
			for (size_t i = 0; i < localConfig.fields.size(); i++)
			{
				m_fields.add(localConfig.fields.at(i).name, std::move(tempValues.at(i)));
			}
		}
	}

	//#OPTIMIZATION make sure u8cmdText is free from the memory

	ValidateInternal();
}

void Command::ValidateInternal()
{
	if (!IsValid())
		return;

	const CommandConfig localConfig = GetConfig();	// Create a local copy of the config info to avoid iterator issues while reading in the loop

	for (const auto& preRegisteredField : localConfig.fields)
	{
		const auto fieldActualValue = m_fields.get_optional<string>(preRegisteredField.name);
		const bool isValueInitialized = fieldActualValue.is_initialized();

		if (preRegisteredField.isRequired && !isValueInitialized)
		{
			LOG << "Command pre-validation (" << localConfig.name << "), required field " << preRegisteredField.name << " is missing" << LogSeverity::warning;
			m_isValid = false;

			// In debug mode we continue the pre-validation process in order to see all the errors
			#ifndef DEBUG
			break;
			#endif
		}

		if (isValueInitialized && preRegisteredField.fieldValueValidator.is_initialized())
		{
			auto validator = preRegisteredField.fieldValueValidator.get();

			if (!validator(fieldActualValue.get()))
			{
				LOG << "Command pre-validation (" << localConfig.name << "), field " << preRegisteredField.name << " is not valid" << LogSeverity::warning;
				m_isValid = false;

				#ifndef DEBUG
				break;
				#endif
			}
		}
	}

	if (!PostValidate())
		m_isValid = false;
}

bool Command::IsValid() const
{
	return m_isValid;
}

boost::optional<string> Command::GetFieldValue(const string& fieldName)
{
	return m_fields.get_optional<string>(fieldName);
}
