#include "stdafx.h"
#include "Common.h"
#include "CommandsManager.h"

class Connect final : public Command
{
public:
	Connect() : Command()
	{
		LOG << "!Connect created ";
	}

	const CommandConfig& GetConfig() const override
	{
		static auto config = CommandConfig();	//TODO have a way to define if a command can be executed from a simple connection, from a user or both (std::bind?)
		config.name = "CONNECT";
		config.format = Format::INLINE;
		config.fields =
		{
			CommmandField { "username", true, FIELD_VALUE_VALIDATOR(int, [](int test) { LOG << LogSeverity::warning << "LAMBDA: " << test; return true; }) },
			CommmandField{ "password", true, FIELD_VALUE_VALIDATOR(string, Connect::Test) }
		};

		return config;
	}

	bool Connect::PostValidate() const override
	{


		return true;
	}

	static bool Test(string str)
	{
		LOG << "WORKS " << str << LogSeverity::debug;
		return true;
	}

};

APOAPSE_COMMAND_REGISTER(Connect, "CONNECT");