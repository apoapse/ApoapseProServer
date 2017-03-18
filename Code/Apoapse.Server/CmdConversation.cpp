#include "stdafx.h"
#include "Common.h"
#include "CommandsManager.h"
#include "LocalUser.h"
#include "Uuid.h"
#include "ApoapseAddress.h"
#include "Conversation.h"
#include "RemoteServer.h"

class CmdConversation final : public Command
{
	std::vector<ApoapseAddress> m_correspondents;

public:
	const CommandConfig& GetConfig() override
	{
		static auto config = CommandConfig();
		config.name = "CONVERSATION";
		config.expectedFormat = Format::JSON;
		config.propagateToUser = true;
		config.processFromUser = PROCESS_METHOD_FROM_USER(CmdConversation::FromInternal);
		config.processFromRemoteServer = PROCESS_METHOD(RemoteServer, CmdConversation::FromExternal);
		config.fields =
		{
			CommandField{ "uuid", FieldRequirement::VALUE_MENDATORY, FIELD_VALUE_VALIDATOR(string, Uuid::IsValid) },
			CommandField{ "correspondents", FieldRequirement::ARRAY_MENDATORY, FIELD_VALUE_VALIDATOR(string, [](string address) { return ApoapseAddress(address).IsValid(); }) }
		};

		return config;
	}

	bool PostValidate() override
	{
		return true;
	}

private:
	bool FromInternal(LocalUser& user, ClientConnection& originConnection)
	{
		auto address = user.GetFullAddress();
		GenerateCorrespondentsList(&address);

		return RegisterConversation(originConnection, user.GetDatabaseId());
	}

	void FromExternal(RemoteServer& remoteServer)
	{
		GenerateCorrespondentsList();

		RegisterConversation(remoteServer);
	}
 
	bool RegisterConversation(GenericConnection& originConnection, DbId associatedUser = 0)
	{
		const auto uuid = Uuid(ReadFieldValue<string>("uuid").get());
		const size_t correspondentCount = m_correspondents.size();

		if (correspondentCount == 0)
		{
			ApoapseError::SendError(ApoapseErrorCode::MALFORMED_CMD, originConnection);
			return false;
		}
		else if (correspondentCount > Conversation::GetMaxAllowedCorespondants())
		{
			ApoapseError::SendError(ApoapseErrorCode::MAX_CONVERSATION_CORESPONDANTS_EXCEEDED, uuid.ToStr(), originConnection);
			return false;
		}

		// #TODO Check if there are users on the local server in the correspondents list which does not exist

		Conversation conversation(uuid, m_correspondents, OperationDirection::RECEIVED, originConnection.server);
		conversation.Send(m_correspondents, originConnection);
		conversation.SaveToDatabase(originConnection);

		if (associatedUser)
			conversation.LogOperation(associatedUser);

		return true;
 	}

	void GenerateCorrespondentsList(const ApoapseAddress* currentUserAddress = nullptr)
	{
		for (const string& addressStr : ReadFieldArray<string>("correspondents"))
		{
			auto address = ApoapseAddress(addressStr);

			if (currentUserAddress == nullptr || address != *currentUserAddress) // Remove the current user own address if present
				m_correspondents.push_back(address);
		}
	}
};

APOAPSE_COMMAND_REGISTER(CmdConversation, "CONVERSATION");