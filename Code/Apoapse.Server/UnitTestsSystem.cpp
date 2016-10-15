#include "stdafx.h"
#include "UnitTestsSystem.h"

#include "UT_SettingsManager.h"
#include "UT_ByteConverter.h"

TestsManager* testsManager = new TestsManager();

void UnitTestsSystem::RegisterUnitTests()
{
	UT_SettingsManager();
	UT_ByteConverter();
}

void UnitTestsSystem::RunTests()
{
	testsManager->RunTests();
	delete testsManager;
}