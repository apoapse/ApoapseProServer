#include "stdafx.h"
#include "Common.h"
#include "CommandsManager.h"

class Message final : public Command
{
public:
	const CommandConfig& GetConfig() override
	{
		static auto config = CommandConfig();
		config.name = "MESSAGE";
		config.expectedFormat = Format::JSON;
		config.isPayloadExpected = true;
		config.processFromClient = PROCESS_METHOD(ClientConnection, [](ClientConnection&) { LOG << "PROCESSING" << LogSeverity::debug; });//TEMPS -> TODELETE
		config.fields =
		{
		};

		return config;
	}

	bool PostValidate() const override
	{
		return true;
	}
};

APOAPSE_COMMAND_REGISTER(Message, "MESSAGE");