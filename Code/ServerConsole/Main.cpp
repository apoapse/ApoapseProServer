#include "stdafx.h"
#include "Apoapse.Server\Common.h"
#include "Apoapse.Server\InternalLibraryLoader.h"

#include <iostream>

Global* global = Global::CreateGlobal();

int main(int argc, char* argv[])
{
	InternalLibraryLoader::LoadInternalLibrary(global, "Apoapse.Server");

	string inputstr;
	getline(std::cin, inputstr);

	return 0;
}