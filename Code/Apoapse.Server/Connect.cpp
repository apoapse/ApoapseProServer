#include "stdafx.h"
#include "Common.h"
#include "CommandsManager.h"

class Connect : public Command
{
public:
	Connect(ApoapseServer& apoapseServer) : Command(apoapseServer)
	{
		LOG << "!Connect created ";
	}

	const CommandConfig& GetConfig() const
	{
		static auto config = CommandConfig();	//TODO have a way to define if a command can be executed from a simple connection, from a user or both (std::bind?)
		config.name = "CONNECT";
		config.format = Format::INLINE;
		config.fields =
		{
			CommmandField<string> { "username", true },
			//CommmandField<string> { "password", true, [](string test) { LOG << "LAMBDA: " << test; return false; } }
			CommmandField<string> { "password", true, Connect::Test }
		};

		return config;
	}

	bool Connect::PostValidate() const
	{

		return true;
	}

	static bool Test(string str)
	{
		LOG << "WORKS";
		return false;
	}

};
APOAPSE_COMMAND_REGISTER(Connect, "CONNECT");