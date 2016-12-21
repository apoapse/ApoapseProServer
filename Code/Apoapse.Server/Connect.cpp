#include "stdafx.h"
#include "Common.h"
#include "CommandsManager.h"
#include "GenericConnection.h"
#include "LocalUser.h"

class Connect final : public Command
{
public:
	Connect() : Command()
	{
		LOG << "!Connect created ";
	}

	const CommandConfig& GetConfig() override
	{
		static auto config = CommandConfig();
		config.name = "CONNECT";
		config.expectedFormat = Format::INLINE;
		config.processFromGenericConnection = PROCESS_METHOD(GenericConnection, Connect::Process);
		config.fields =
		{
			CommmandField { "username", true },
			CommmandField{ "password", true, FIELD_VALUE_VALIDATOR(string, Connect::Test) }
		};

		return config;
	}

	bool PostValidate() const override
	{
		return true;
	}

	void Process(GenericConnection* connection)
	{
		if (ReadFieldValue<string>("username").get() == "Guillaume" && ReadFieldValue<string>("password").get() == "MyPassword")
		{
			connection->SetAssociatedActor(std::make_shared<LocalUser>());
			LOG << "User " << ReadFieldValue<string>("username").get() << " connected with success";
		}
		else
		{
			LOG << "Wrong username or password" << LogSeverity::warning;
		}
	}

	static bool Test(string str)
	{
		LOG << "WORKS " << str << LogSeverity::debug;
		return true;
	}

};

APOAPSE_COMMAND_REGISTER(Connect, "CONNECT");