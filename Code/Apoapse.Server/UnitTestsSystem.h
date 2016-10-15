#pragma once
#include "Apoapse.UnitTest\TestsManager.h"

extern TestsManager* testsManager;

namespace UnitTestsSystem
{
	void RegisterUnitTests();
	void RunTests();
}