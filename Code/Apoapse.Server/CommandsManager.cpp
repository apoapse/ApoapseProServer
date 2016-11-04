#include "stdafx.h"
#include "CommandsManager.h"
#include "Common.h"

CommandsManager::CommandsManager()
{

}

CommandsManager::~CommandsManager()
{

}

std::shared_ptr<Command> CommandsManager::CreateCommand(string commandName)
{
	ASSERT_MSG(CommandExist(commandName), "The command requested does not exist");

	return m_commandFactories[commandName]->Create();
}

bool CommandsManager::CommandExist(string commandName) const
{
	return (m_commandFactories.count(commandName) > 0);
}