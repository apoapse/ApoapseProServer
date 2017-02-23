#include "stdafx.h"
#include "Common.h"
#include "CommandsManager.h"
#include "LocalUser.h"
#include "Uuid.h"
#include "ApoapseAddress.h"
#include "Conversation.h"

class CmdConversation final : public Command
{
	std::vector<ApoapseAddress> m_correspondents;

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

		GenerateCorrespondentsList(user.GetFullAddress());
		const size_t correspondentCount = m_correspondents.size();

		if (correspondentCount == 0)
		{
			ApoapseError::SendError(ApoapseErrorCode::MALFORMED_CMD, originConnection);
			return;
		}
		else if (correspondentCount > Conversation::GetMaxAllowedCorespondants())
		{
			ApoapseError::SendError(ApoapseErrorCode::MAX_CONVERSATION_CORESPONDANTS_EXCEEDED, uuid.GetStr(), originConnection);
			return;
		}

		Conversation conversation(uuid, m_correspondents, OperationDirection::RECEIVED, originConnection.server);
		conversation.SaveToDatabase(user, originConnection);
	}

	void GenerateCorrespondentsList(const ApoapseAddress& currentUserAddress)
	{
		for (const string& addressStr : ReadFieldArray<string>("correspondents"))
		{
			auto address = ApoapseAddress(addressStr);

			if (address != currentUserAddress) // Remove the current user own address if present
				m_correspondents.push_back(address);
		}
	}
};

APOAPSE_COMMAND_REGISTER(CmdConversation, "CONVERSATION");