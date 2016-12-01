#include "TestsManager.h"
#include <iostream>
#include <string>
#include <algorithm>
#include <cassert>

#ifdef _WIN32
#include <windows.h>
#endif // _WIN32

TestsManager::TestsManager()
{
}

TestsManager::~TestsManager()
{

}

UNIT_TESTS_DLL_API void TestsManager::RunTests(const char* /*testsPath = ""*/)	// #TODO support url type address to trigger specific tests or group of tests
{
	int successCount = 0;
	int errorsCount = 0;
	int toExecuteTestsCount = (int)m_registeredUnitTests.size();

	Log("EXECUTING " + std::to_string(toExecuteTestsCount) + " UNIT TESTS...");

	for (const UnitTest& test : m_registeredUnitTests)
	{
		string errorMsg = "";
		bool testResult = test.Execute(errorMsg);

		if (testResult)
		{
			Log("TEST " + std::string(test.GetFullName()) + " -> SUCCESS", ConsoleColors::green);
			++successCount;
		}
		else
		{
			Log("TEST " + std::string(test.GetFullName()) + " -> FAILURE " + errorMsg, ConsoleColors::red);
			++errorsCount;
		}
	}

	Log("EXECUTED " + std::to_string(toExecuteTestsCount) + " UNIT TESTS. " + std::to_string(successCount) + " successful, " + std::to_string(errorsCount) + " failed");
}

void TestsManager::RegisterTest(const UnitTest& test)
{
	assert(!(std::find(m_registeredUnitTests.begin(), m_registeredUnitTests.end(), test) != m_registeredUnitTests.end()) && "Test already registered");

	m_registeredUnitTests.push_back(test);
}

void TestsManager::Log(const string& msg, ConsoleColors color) const
{
#ifdef _WIN32
	switch (color)
	{
	case TestsManager::green:
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_GREEN | FOREGROUND_INTENSITY);
		break;
	case TestsManager::red:
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED | FOREGROUND_INTENSITY);
		break;
	}
#endif
	
	std::cout << msg << '\n';

#ifdef _WIN32
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);	// Reset to default color
#endif
}