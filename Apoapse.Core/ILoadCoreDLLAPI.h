#pragma once
#include "Common.h"

struct ILoadCoreDLLAPI
{
	virtual void Start(Global*) = 0;
};