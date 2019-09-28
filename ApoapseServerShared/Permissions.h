#pragma once
#include "Uuid.h"

namespace ApoapsePermissions
{
	bool IsUserAllowedToReadMessage(const Username& user, const Uuid& message);
	bool IsUserAllowedToReadMessage(const Username& user, DataStructure& message);
}