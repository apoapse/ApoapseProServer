#pragma once
#include "Types.h"
#include "INetworkSender.h"
#include "CommandsManager.h"

enum class ApoapseErrorCode : UInt16
{
	MALFORMED_CMD 									= 2,
	UNAUTHORIZED_ACTION								= 3,
	WRONG_LOGIN_CREDENTIALS							= 4,
	INTERNAL_SERVER_ERROR							= 5,
	MAX_CONVERSATION_CORESPONDANTS_EXCEEDED			= 6,
	ITEM_UUID_ALREADY_REGISTERED					= 7,
	DATETIME_MISMATCH								= 8,
	CONVERSATION_NONEEXISTENT						= 9
};

class ApoapseError
{
public:
	inline static void SendError(ApoapseErrorCode error, INetworkSender& destination)
	{
		std::unique_ptr<Command> command = CommandsManager::GetInstance().CreateCommand("ERROR");
		command->InsertFieldValue<UInt16>("error_code", (UInt16)error);

		command->Send(destination);
	}

	template <typename T>
	inline static void SendError(ApoapseErrorCode error, const T& relatedItem, INetworkSender& destination)
	{
		std::unique_ptr<Command> command = CommandsManager::GetInstance().CreateCommand("ERROR");
		command->InsertFieldValue<UInt16>("error_code", (UInt16)error);
		command->InsertFieldValue<T>("related_item", relatedItem);

		command->Send(destination);
	}

private:
	static void LogError()
	{
		// #TODO
	}
};