#include "stdafx.h"
#include "Common.h"
#include "CommandsManager.h"
#include "LocalUser.h"
#include "Uuid.h"
#include "ApoapseAddress.h"

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
			CommandField{ "uuid", FieldRequirement::VALUE_MENDATORY, FIELD_VALUE_VALIDATOR(string, Uuid::IsValid) },
			CommandField{ "correspondents", FieldRequirement::ARRAY_MENDATORY/*, FIELD_VALUE_VALIDATOR(string, ApoapseAddress::IsValid())*/ }
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
		
		auto test = ReadFieldArray<string>("correspondents");

		InsertFieldArray<int>("testarr", std::vector<int> { -1, 5 });
		int fe = 1;
	}
};

APOAPSE_COMMAND_REGISTER(CmdConversation, "CONVERSATION");