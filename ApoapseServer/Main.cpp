#include "stdafx.h"
#include "Common.h"
#include "Logger.h"
#include <boost/dll/import.hpp>
#include "ILoadCoreDLLAPI.h"

Global* global = Global::CreateGlobal();

int main(int argc, char* argv[])
{

	boost::shared_ptr<ILoadCoreDLLAPI> coreDLL;
	try
	{
		coreDLL = boost::dll::import<ILoadCoreDLLAPI>("Apoapse.Core", "core_dll", boost::dll::load_mode::append_decorations);
	}
	catch (const std::exception& e)
	{
		printf(e.what());	//FATAL ERROR
	}

	coreDLL->Start(global);

	global->settings->RegisterConfigVar<string>("general.test_string", "AAaA");
	global->logger->Log(global->settings->ReadConfigValue<string>("general.test_string"), LogSeverity::debug);
	
	string inputstr;
	getline(std::cin, inputstr);

	return 0;
}