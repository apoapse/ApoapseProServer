#pragma once
class Command;

class Actor
{
	

public:
	Actor();
	virtual ~Actor();

	virtual void ProcessCommandFromNetwork(Command& command) = 0;
	virtual bool IsCommandCompatibleWithCurrentActor(Command& command) = 0;
	
private:
};