#pragma once
#include "Common.h"

struct InternalLibraryLoadingAPI
{
	virtual void Start(Global*) = 0;
};