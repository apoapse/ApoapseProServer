#include "stdafx.h"
#include "Common.h"
#include "CommandsManager.h"
#include "ClientConnection.h"

class CmdMessage final : public Command
{
public:
	const CommandConfig& GetConfig() override
	{
		static auto config = CommandConfig();
		config.name = "MESSAGE";
		config.expectedFormat = Format::JSON;
		config.isPayloadExpected = true;
		config.processFromClient = PROCESS_METHOD(ClientConnection, CmdMessage::Process);
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

APOAPSE_COMMAND_REGISTER(CmdMessage, "MESSAGE");