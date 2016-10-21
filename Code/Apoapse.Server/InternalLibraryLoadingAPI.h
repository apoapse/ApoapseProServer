#pragma once
#include "Common.h"

struct InternalLibraryLoadingAPI
{
	virtual void Start(Global*, std::vector<std::string>& params) = 0;
};