#pragma once
#include "Types.h"
#include "INetworkSender.h"
#include "CommandsManager.h"

enum class ApoapseErrorCode : UInt16
{
	MALFORMED_CMD 									= 2,
	UNAUTHORIZED_ACTION								= 3,
	WRONG_LOGIN_CREDENTIALS							= 4,
	INTERNAL_SERVER_ERROR							= 5
};

namespace ApoapseError
{
	inline void SendError(ApoapseErrorCode error, INetworkSender& destination)
	{
		std::unique_ptr<Command> command = CommandsManager::GetInstance().CreateCommand("ERROR");
		command->InsertFieldValue<UInt16>("error_code", (UInt16)error);

		command->Send(destination);
	}

	template <typename T>
	inline void SendError(ApoapseErrorCode error, const T& relatedItem, INetworkSender& destination)
	{
		std::unique_ptr<Command> command = CommandsManager::GetInstance().CreateCommand("ERROR");
		command->InsertFieldValue<UInt16>("error_code", (UInt16)error);
		command->InsertFieldValue<T>("related_item", relatedItem);

		command->Send(destination);
	}
}