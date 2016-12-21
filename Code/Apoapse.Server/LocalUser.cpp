#include "stdafx.h"
#include "LocalUser.h"
#include "Common.h"

LocalUser::LocalUser(/*const Address& address*/)
{

}

LocalUser::~LocalUser()
{

}

void LocalUser::ProcessCommandFromNetwork(Command& command)
{
	command.ProcessFromNetwork(this);
}

bool LocalUser::IsCommandCompatibleWithCurrentActor(Command& command)
{
	return command.CanProcessThisActor(this);
}