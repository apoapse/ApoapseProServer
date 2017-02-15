#include "stdafx.h"
#include "Common.h"
#include "CommandsManager.h"
#include "LocalUser.h"
#include "Uuid.h"

class CmdConversation final : public Command
{
public:
	const CommandConfig& GetConfig() override
	{
		static auto config = CommandConfig();
		config.name = "CONVERSATION";
		config.expectedFormat = Format::JSON;
		config.processFromUser = PROCESS_METHOD_FROM_USER(CmdConversation::RegisterConversation);
		config.fields =
		{
			CommmandField{ "uuid", true, FIELD_VALUE_VALIDATOR(string, Uuid::IsValid) }
		};

		return config;
	}

	bool PostValidate() const override
	{
		return true;
	}

private:
	void RegisterConversation(LocalUser& user, ClientConnection& originConnection)
	{
		
		auto test = ReadFieldValue<string>("correspondents").get();

		int fe = 1;
	}
};

APOAPSE_COMMAND_REGISTER(CmdConversation, "CONVERSATION");