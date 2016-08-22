#include "stdafx.h"
#include "Apoapse.Core\Common.h"
#include "Apoapse.Core\InternalLibraryLoader.h"

Global* global = Global::CreateGlobal();

int main(int argc, char* argv[])
{
	InternalLibraryLoader::LoadInternalLibrary(global, "Apoapse.Core");
	InternalLibraryLoader::LoadInternalLibrary(global, "Apoapse.Network");

	global->settings->RegisterConfigVar_string("general.test_string", "AAaA");
	auto test = global->settings->ReadConfigValue_string("general.test_string");
	Log(test, LogSeverity::debug);

	string inputstr;
	getline(std::cin, inputstr);

	return 0;
}