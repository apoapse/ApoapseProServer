#pragma once
#include "Uuid.h"

class ApoapseMessage
{
	

public:
	static bool DoesMessageExist(const Uuid& uuid);
	static DbId GetMessageDbIdByUuid(const Uuid& uuid);
	static Uuid GetMessageUuidByDbId(DbId dbId);

private:
};