#include "stdafx.h"
#include "UnitTestsSystem.h"

#include "UT_SettingsManager.h"
#include "UT_ByteConverter.h"
#include "UT_NetMessage.h"

TestsManager* testsManager = new TestsManager();

void UnitTestsSystem::RegisterUnitTests()
{
	UT_SettingsManager();
	UT_ByteConverter();
	UT_NetMessage();
}

void UnitTestsSystem::RunTests()
{
	testsManager->RunTests();
	delete testsManager;
}