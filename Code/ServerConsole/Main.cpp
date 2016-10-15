#include "stdafx.h"
#include "Apoapse.Server\Common.h"
#include "Apoapse.Server\InternalLibraryLoader.h"

#include <iostream>

Global* global = Global::CreateGlobal();

int main(int argcout, char* argv[])
{
	InternalLibraryLoader::LoadInternalLibrary(global, "Apoapse.Server");

	//Log(string(argv[0]));

	string inputstr;
	getline(std::cin, inputstr);

	return 0;
}