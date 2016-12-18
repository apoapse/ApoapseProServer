#pragma once
#include "Command.h"
#include <map>

// OBJECT FACTORY
struct ICommandFactory
{
	virtual std::unique_ptr<Command> Create() = 0;
};

template<class T_COMMAND>
class CommmandFactory : public ICommandFactory
{
public:
	std::unique_ptr<Command> Create()
	{
		return std::make_unique<T_COMMAND>();
	}
};

// OBJECT FACTORY AUTO REGISTRATION
template<class T_COMMAND>
class CommandAutoRegister
{
public:
	CommandAutoRegister(const string& name)
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

	std::unique_ptr<Command> CreateCommand(const string& commandName);
	bool CommandExist(const string& commandName) const;
};