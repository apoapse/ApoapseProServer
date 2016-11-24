#pragma once
#include "InternalLibraryLoadingAPI.h"
#include <boost/config.hpp>

class ApoapseServerStartup : public InternalLibraryLoadingAPI
{
public:
	ApoapseServerStartup()
	{
	}
	~ApoapseServerStartup();

	void Start(std::vector<std::string>& params);
};

extern "C" BOOST_SYMBOL_EXPORT ApoapseServerStartup apoapse_internal_dll;
ApoapseServerStartup apoapse_internal_dll;