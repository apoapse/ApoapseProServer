#include "stdafx.h"
#include "Apoapse.Core\Common.h"
#include "Apoapse.Core\InternalLibraryLoader.h"

Global* global = Global::CreateGlobal();

int main(int argc, char* argv[])
{
	InternalLibraryLoader::LoadInternalLibrary(global, "Apoapse.Core");

	global->settings->RegisterConfigVar<string>("general.test_string", "AAaA");
	Log(global->settings->ReadConfigValue<string>("general.test_string"), LogSeverity::debug);

	string inputstr;
	getline(std::cin, inputstr);

	return 0;
}