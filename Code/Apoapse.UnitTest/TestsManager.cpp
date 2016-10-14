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

void TestsManager::StartTests(const char* testsPath)	// #TODO support url type address to trigger specific tests or group of tests
{
	Log("EXECUTING " + std::to_string(m_registeredUnitTests.size()) + " UNIT TESTS...");

	for (size_t i = 0; i < m_registeredUnitTests.size(); i++)
	{
		char* errorMsg = "";
		auto unitTest = m_registeredUnitTests.at(i);
		const bool testResult = unitTest->Execute(errorMsg);

		if (testResult)
			Log("TEST " + std::string(unitTest->GetFullName()) + " -> SUCCESS", ConsoleColors::green);
		else
			Log("TEST " + std::string(unitTest->GetFullName()) + " -> ERROR " + string(errorMsg), ConsoleColors::red);
	}
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