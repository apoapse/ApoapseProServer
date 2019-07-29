#include <iostream>
#include <vector>
#include <string>
#include "ServerEntryPoint.h"

#ifdef _WIN32
#include <windows.h>
#endif

int main(int argcount, char* argv[])
{
#ifdef _WIN32
	SetConsoleTitle("Apoapse Server");
#endif

	ServerMain(std::vector<std::string>(argv, argv + argcount));

	//std::string inputstr;
	//getline(std::cin, inputstr);

	return 0;
}