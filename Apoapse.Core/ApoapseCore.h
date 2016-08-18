#pragma once
#include "InternalLibraryLoadingAPI.h"
#include <boost/config.hpp>

class ApoapseCore : public InternalLibraryLoadingAPI
{
public:
	ApoapseCore()
	{
	}
	~ApoapseCore();

	void Start(Global*);
private:

};

extern "C" BOOST_SYMBOL_EXPORT ApoapseCore apoapse_internal_dll;
ApoapseCore apoapse_internal_dll;