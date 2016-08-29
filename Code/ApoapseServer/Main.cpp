#include "stdafx.h"
#include "Apoapse.Core\Common.h"
#include "Apoapse.Core\InternalLibraryLoader.h"

Global* global = Global::CreateGlobal();

int main(int argc, char* argv[])
{
	InternalLibraryLoader::LoadInternalLibrary(global, "Apoapse.Core");
	InternalLibraryLoader::LoadInternalLibrary(global, "Apoapse.Network");

	string inputstr;
	getline(std::cin, inputstr);

	return 0;
}