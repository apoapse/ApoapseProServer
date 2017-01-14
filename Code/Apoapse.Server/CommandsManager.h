#pragma once
#include "Command.h"
#include "ObjectsFactory.h"

#define APOAPSE_COMMAND_REGISTER(_class, _name)	OBJECTS_FACTORY_REGISTER(CommandsManager::GetInstance().commandsFactory, Command, _class, string, _name)

class CommandsManager
{
	CommandsManager();
	~CommandsManager();

public:
	ObjectsFactory<string, Command> commandsFactory;

	CommandsManager(CommandsManager const&) = delete;
	void operator=(CommandsManager const&) = delete;

	static CommandsManager& GetInstance()
	{
		static CommandsManager commandsManager;
		return commandsManager;
	}

	std::unique_ptr<Command> CreateCommand(const string& commandName);
	bool CommandExist(const string& commandName) const;
};