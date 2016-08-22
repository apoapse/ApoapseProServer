#pragma once
#include "Common.h"
#include "InternalLibraryLoadingAPI.h"
#include <boost/config.hpp>

class ApoapseNetwork : public InternalLibraryLoadingAPI
{
public:
	ApoapseNetwork()
	{
	}
	~ApoapseNetwork();

	void Start(Global* outsideGlobalPtr);
};

extern "C" BOOST_SYMBOL_EXPORT ApoapseNetwork apoapse_internal_dll;
ApoapseNetwork apoapse_internal_dll;