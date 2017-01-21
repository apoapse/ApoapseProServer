#include "stdafx.h"
#include "Apoapse.Server\Common.h"
#include "Apoapse.Server\InternalLibraryLoadingAPI.h"
#include "Apoapse.Server\InternalLibraryLoader.h"

#include <iostream>

int main(int argcount, char* argv[])
{
	auto dll = InternalLibraryLoader::LoadInternalLibrary<InternalLibraryLoadingAPI>("Apoapse.Server");
	dll->Start(std::vector<string>(argv, argv + argcount));

	string inputstr;
	getline(std::cin, inputstr);

	return 0;
}