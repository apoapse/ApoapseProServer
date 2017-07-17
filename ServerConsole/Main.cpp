#include <iostream>
#include <vector>
#include <string>
#include "ServerEntryPoint.h"

int main(int argcount, char* argv[])
{
	ServerMain(std::vector<std::string>(argv, argv + argcount));

	std::string inputstr;
	getline(std::cin, inputstr);

	return 0;
}