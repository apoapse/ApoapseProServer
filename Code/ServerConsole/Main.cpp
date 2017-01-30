#include "stdafx.h"
#include <iostream>
#include <vector>
#include <string>
#include "Apoapse.Server\InternalLibraryLoadingAPI.h"
#include "Apoapse.Server\InternalLibraryLoader.h"

int main(int argcount, char* argv[])
{
	auto dll = InternalLibraryLoader::LoadInternalLibrary<InternalLibraryLoadingAPI>("Apoapse.Server");
	dll->Start(std::vector<std::string>(argv, argv + argcount));

	std::string inputstr;
	getline(std::cin, inputstr);

	return 0;
}