#include "stdafx.h"
#include "Apoapse.Server\Common.h"
#include "Apoapse.Server\InternalLibraryLoader.h"

#include <iostream>

int main(int argcount, char* argv[])
{
	InternalLibraryLoader::LoadInternalLibrary("Apoapse.Server", std::vector<string>(argv, argv + argcount));

	string inputstr;
	getline(std::cin, inputstr);

	return 0;
}