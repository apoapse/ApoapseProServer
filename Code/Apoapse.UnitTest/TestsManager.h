#pragma once
#include "Apoapse.UnitTest/UnitTest.h"
#include <vector>

#ifdef COMPILE_DLL
#define UNIT_TESTS_DLL_API  __declspec(dllexport)
#else
#define UNIT_TESTS_DLL_API __declspec(dllimport)
#endif

class TestsManager
{
	using string = std::string;
private:
	std::vector<UnitTest> m_registeredUnitTests;

	enum ConsoleColors
	{
		default,
		green,
		red
	};

public:
	UNIT_TESTS_DLL_API TestsManager();
	UNIT_TESTS_DLL_API virtual ~TestsManager();


	UNIT_TESTS_DLL_API void RunTests(const char* /*testsPath*/ = "");
	UNIT_TESTS_DLL_API void RegisterTest(const UnitTest& test);

	TestsManager(TestsManager const&) = delete;
	void operator=(TestsManager const&) = delete;

	static TestsManager& GetInstance()
	{
		static TestsManager testsManager;
		return testsManager;
	}

private:
	UNIT_TESTS_DLL_API void Log(const string& msg, ConsoleColors color = ConsoleColors::default) const;
};

class UnitTestAutoRegister
{
public:
	UnitTestAutoRegister(const UnitTest& test)
	{
		TestsManager::GetInstance().RegisterTest(test);
	}
};

#define CONCAT_IMPL( x, y ) x##y
#define MACRO_CONCAT( x, y ) CONCAT_IMPL( x, y )

#define UNIT_TEST(_name)	static UnitTestAutoRegister MACRO_CONCAT(testRegister_, __COUNTER__)(UnitTest(_name, []() -> void
#define UNIT_TEST_END		));