#pragma once
#include "stdafx.h"

#ifdef UNIT_TESTS

#include "Common.h"
#include "Apoapse.UnitTest/TestsManager.h"
#include "Uuid.h"

UNIT_TEST("uuid:Generate")
{
	UnitTest::Assert(Uuid::IsValid(Uuid::Generate()));
} UNIT_TEST_END

UNIT_TEST("uuid:Object")
{
	Uuid uuid("81128ddee533436a9a302c8767bcd458");

	UnitTest::Assert(Uuid::IsValid(uuid.GetStr()));
} UNIT_TEST_END

UNIT_TEST("uuid:Validate")
{
	UnitTest::Assert(Uuid::IsValid("81128ddee533436a9a302c8767bcd458"));
} UNIT_TEST_END

UNIT_TEST("uuid:Validate:WrongValue")
{
	UnitTest::Assert(!Uuid::IsValid("6646c75e-bc03-4466-9a62-115ff03f13fe"));
} UNIT_TEST_END

#endif	// UNIT_TESTS