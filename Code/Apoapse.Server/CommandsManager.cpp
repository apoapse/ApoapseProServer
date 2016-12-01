#include "stdafx.h"
#include "CommandsManager.h"
#include "Common.h"

CommandsManager::CommandsManager()
{

}

CommandsManager::~CommandsManager()
{

}

std::shared_ptr<Command> CommandsManager::CreateCommand(const string& commandName, ApoapseServer& apoapseServer)
{
	ASSERT_MSG(CommandExist(commandName), "The command requested does not exist");	//TODO exception

	return m_commandFactories[commandName]->Create(apoapseServer);
}

bool CommandsManager::CommandExist(const string& commandName) const
{
	return (m_commandFactories.count(commandName) > 0);
}