#include "stdafx.h"
#include "Common.h"
#include "CommandsManager.h"
#include "ClientConnection.h"
#include "LocalUser.h"
#include "ApoapseAddress.h"

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
		config.processFromClient = PROCESS_METHOD(ClientConnection, Connect::Process);
		config.fields =
		{
			CommmandField { "username", true, FIELD_VALUE_VALIDATOR(string, ApoapseAddress::UsernameHash::IsValid) },
			CommmandField{ "password", true, FIELD_VALUE_VALIDATOR(string, Connect::Test) }
		};

		return config;
	}

	bool PostValidate() const override
	{
		return true;
	}

	void Process(ClientConnection& connection)
	{
		// #TODO_DATABASE_IMPL probably on the user class or connectedUsersManager (in this case, this should be renamed to UsersManager)

		if (ApoapseAddress::UsernameHash(ReadFieldValue<string>("username").get()) == ApoapseAddress::UsernameHash("9f86d081884c7d659a2feaa0c55ad015a3bf4f1b2b0b822cd15d6c15b0f00a08") && ReadFieldValue<string>("password").get() == "MyPassword")
		{
			connection.SetAssociatedUser(std::make_shared<LocalUser>());	// TODO: Use a user manager to make sure an user object can't be created two times if connected from two different connections. (maybe use a general actor manager if necessary for the remote server as well)
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