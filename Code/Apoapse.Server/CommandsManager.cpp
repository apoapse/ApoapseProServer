#include "stdafx.h"
#include "CommandsManager.h"
#include "Common.h"

CommandsManager::CommandsManager()
{

}

CommandsManager::~CommandsManager()
{

}

std::unique_ptr<Command> CommandsManager::CreateCommand(const string& commandName)
{
	ASSERT_MSG(CommandExist(commandName), "The command requested does not exist");

	return commandsFactory.CreateObject(commandName);
}

bool CommandsManager::CommandExist(const string& commandName) const
{
	return commandsFactory.IsObjectExist(commandName);
}