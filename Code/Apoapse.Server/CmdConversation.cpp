#include "stdafx.h"
#include "Common.h"
#include "CommandsManager.h"
#include "LocalUser.h"
#include "Uuid.h"
#include "ApoapseAddress.h"
#include "Conversation.h"

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
			CommandField{ "correspondents", FieldRequirement::ARRAY_MENDATORY, FIELD_VALUE_VALIDATOR(string, [](string address) { return ApoapseAddress(address).IsValid(); }) }
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
		const auto uuid = Uuid(ReadFieldValue<string>("uuid").get());
		const std::vector<string>& correspondantsStr = ReadFieldArray<string>("correspondents");

		if (correspondantsStr.size() > Conversation::GetMaxAllowedCorespondants())
		{
			ApoapseError::SendError(ApoapseErrorCode::MAX_CONVERSATION_CORESPONDANTS_EXCEEDED, uuid.GetStr(), originConnection);
			return;
		}

		std::vector<ApoapseAddress> correspondants;

		for (const string& address : correspondantsStr)
			correspondants.push_back(ApoapseAddress(address));

		Conversation conversation(uuid, correspondants, OperationDirection::RECEIVED, originConnection.server);
		conversation.SaveToDatabase(user, originConnection);
	}
};

APOAPSE_COMMAND_REGISTER(CmdConversation, "CONVERSATION");