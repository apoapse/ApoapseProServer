#pragma once
#include <boost/dll/import.hpp>
#include "InternalLibraryLoadingAPI.h"
#include <iostream>

struct InternalLibraryLoader
{
	static void LoadInternalLibrary(const char* libraryName, std::vector<std::string>& params)
	{
		boost::shared_ptr<InternalLibraryLoadingAPI> coreDLL;
		try
		{
			coreDLL = boost::dll::import<InternalLibraryLoadingAPI>(libraryName, "apoapse_internal_dll", boost::dll::load_mode::append_decorations);
		}
		catch (const std::exception&)
		{
			std::cout << "Unable to load the dynamic library " << string(libraryName);
			std::abort();
		}

		coreDLL->Start(params);
	}
};