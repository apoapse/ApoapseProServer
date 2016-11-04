#pragma once
#include "Command.h"
#include <map>

// OBJECT FACTORY
struct ICommandFactory
{
	virtual std::shared_ptr<Command> Create() = 0;
};

template<class T_COMMAND>
class CommmandFactory : public ICommandFactory
{
public:
	std::shared_ptr<Command> Create()
	{
		return std::make_shared<T_COMMAND>();
	}
};

// OBJECT FACTORY AUTO REGISTRATION
template<class T_COMMAND>
class CommandAutoRegister
{
public:
	CommandAutoRegister(string name)
	{
		CommandsManager::GetInstance().RegisterCommand<T_COMMAND>(name);
	}
};
#define APOAPSE_COMMAND_REGISTER(_class, _name)	static CommandAutoRegister<_class> cmdRegister(_name);

class CommandsManager
{
	std::map<string, std::unique_ptr<ICommandFactory>> m_commandFactories;

	CommandsManager();
	~CommandsManager();

public:
	CommandsManager(CommandsManager const&) = delete;
	void operator=(CommandsManager const&) = delete;

	static CommandsManager& GetInstance()
	{
		static CommandsManager commandsManager;
		return commandsManager;
	}

	template<typename T_COMMAND>
	void RegisterCommand(string commandName)
	{
		ASSERT_MSG(!CommandExist(commandName), "This command is already registered");

		m_commandFactories[commandName] = std::make_unique<CommmandFactory<T_COMMAND>>();
	}

	std::shared_ptr<Command> CreateCommand(string commandName);
	bool CommandExist(string commandName) const;
};