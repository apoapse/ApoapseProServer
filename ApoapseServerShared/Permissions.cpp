#include "stdafx.h"
#include "Common.h"
#include "Permissions.h"
#include "Username.h"

bool ApoapsePermissions::IsUserAllowedToReadMessage(const Username& user, const Uuid& message)
{
	DataStructure dat = global->apoapseData->ReadItemFromDatabase("message", "uuid", message);

	return IsUserAllowedToReadMessage(user, dat);
}

bool ApoapsePermissions::IsUserAllowedToReadMessage(const Username& user, DataStructure& message)
{
	if (message.GetField("direct_recipient").HasValue())
	{
		if (message.GetField("author").GetValue<Username>() == user || message.GetField("direct_recipient").GetValue<Username>() == user)
			return true;
		else
			return false;
	}

	return true;
}
