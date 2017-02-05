#include "stdafx.h"
#include "Common.h"
#include "CommandsManager.h"
#include "ClientConnection.h"

class Message final : public Command
{
public:
	const CommandConfig& GetConfig() override
	{
		static auto config = CommandConfig();
		config.name = "MESSAGE";
		config.expectedFormat = Format::JSON;
		config.isPayloadExpected = true;
		config.processFromClient = PROCESS_METHOD(ClientConnection, Message::Process);
		config.fields =
		{
		};

		return config;
	}

	bool PostValidate() const override
	{
		return true;
	}

private:

	void Process(ClientConnection& client)
	{
		LOG << "PROCESSING" << LogSeverity::debug;

		Send(client);
	}
};

APOAPSE_COMMAND_REGISTER(Message, "MESSAGE");