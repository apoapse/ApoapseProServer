#include "TestsManager.h"
#include <iostream>
#include <string>

#include <windows.h>

TestsManager::TestsManager()
{
}

TestsManager::~TestsManager()
{
	for (size_t i = 0; i < m_registeredUnitTests.size(); i++)
		delete m_registeredUnitTests.at(i);
}

void TestsManager::RunTests(const char* testsPath)	// #TODO support url type address to trigger specific tests or group of tests
{
	int successCount = 0;
	int errorsCount = 0;
	int toExecuteTestsCount = (int)m_registeredUnitTests.size();

	Log("EXECUTING " + std::to_string(toExecuteTestsCount) + " UNIT TESTS...");

	for (size_t i = 0; i < m_registeredUnitTests.size(); i++)
	{
		string errorMsg = "";
		const UnitTest* unitTest = m_registeredUnitTests.at(i);
		bool testResult = unitTest->Execute(errorMsg);

		if (testResult)
		{
			Log("TEST " + std::string(unitTest->GetFullName()) + " -> SUCCESS", ConsoleColors::green);
			++successCount;
		}
		else
		{
			Log("TEST " + std::string(unitTest->GetFullName()) + " -> FAILURE " + errorMsg, ConsoleColors::red);
			++errorsCount;
		}
	}

	Log("EXECUTED " + std::to_string(toExecuteTestsCount) + " UNIT TESTS. " + std::to_string(successCount) + " successful, " + std::to_string(errorsCount) + " failed");
}

void TestsManager::RegisterTest(const UnitTest* test)
{
	m_registeredUnitTests.push_back(test);
}

void TestsManager::Log(const string& msg, ConsoleColors color) const
{
	switch (color)
	{
	case TestsManager::green:
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_GREEN | FOREGROUND_INTENSITY);
		break;
	case TestsManager::red:
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED | FOREGROUND_INTENSITY);
		break;
	}
	
	std::cout << msg << '\n';

	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);	// Reset to default color
}