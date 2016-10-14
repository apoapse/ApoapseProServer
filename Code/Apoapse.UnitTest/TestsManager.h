#pragma once
#include "UnitTest.h"
#include <vector>

#ifdef COMPILE_DLL
#define UNIT_TESTS_DLL_API  __declspec(dllexport)
#else
#define UNIT_TESTS_DLL_API __declspec(dllimport)
#endif

class TestsManager
{
	typedef std::function<void()> lamdba;
	using string = std::string;
private:
	std::vector<const UnitTest*> m_registeredUnitTests;
	enum ConsoleColors
	{
		default,
		green,
		red
	};

public:
	UNIT_TESTS_DLL_API TestsManager();
	~TestsManager();

	UNIT_TESTS_DLL_API void StartTests(const char* testsPath = "");
	UNIT_TESTS_DLL_API void RegisterTest(const UnitTest* test);

private:
	void Log(const string& msg, ConsoleColors color = ConsoleColors::default) const;
};