// ----------------------------------------------------------------------------
// Copyright (C) 2020 Apoapse
// Copyright (C) 2020 Guillaume Puyal
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at http://www.apache.org/licenses/LICENSE-2.0
//
// For more information visit https://github.com/apoapse/
// And https://apoapse.space/
// ----------------------------------------------------------------------------

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
