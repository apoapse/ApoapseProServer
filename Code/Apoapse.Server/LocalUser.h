#pragma once
#include "GenericConnection.h"
#include "Actor.h"

class LocalUser : public Actor
{
	std::vector<std::shared_ptr<GenericConnection>> m_connections;

public:
	LocalUser(/*const Address& address*/);
	virtual ~LocalUser();

	virtual void ProcessCommandFromNetwork(Command& command) override;
	virtual bool IsCommandCompatibleWithCurrentActor(Command& command) override;
	
private:
};