#pragma once
#include "InternalLibraryLoadingAPI.h"
#include <boost/config.hpp>

class ApoapseServer : public InternalLibraryLoadingAPI
{
public:
	ApoapseServer()
	{
	}
	~ApoapseServer();

	void Start(Global* outsideGlobalPtr, std::vector<std::string>& params);
};

extern "C" BOOST_SYMBOL_EXPORT ApoapseServer apoapse_internal_dll;
ApoapseServer apoapse_internal_dll;