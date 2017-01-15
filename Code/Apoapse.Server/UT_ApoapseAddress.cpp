#include "stdafx.h"

#ifdef UNIT_TESTS

#include "Common.h"
#include "Apoapse.UnitTest/TestsManager.h"
#include "ApoapseAddress.h"

UNIT_TEST("ApoapseAddress:UsernameHash:Validate")
{
	UnitTest::Assert(ApoapseAddress::UsernameHash::IsValid("9f86d081884c7d659a2feaa0c55ad015a3bf4f1b2b0b822cd15d6c15b0f00a08"));
} UNIT_TEST_END

UNIT_TEST("ApoapseAddress:UsernameHash:ValidateWrongValue")
{
	UnitTest::Assert(!ApoapseAddress::UsernameHash::IsValid("zqsgfsgegfhezd"));
} UNIT_TEST_END

UNIT_TEST("ApoapseAddress:ServerDomain:Validate")
{
	UnitTest::Assert(ApoapseAddress::ServerDomain::IsValid("apoapse.space"));
} UNIT_TEST_END

UNIT_TEST("ApoapseAddress:ServerDomain:ValidateWrongValue")
{
	UnitTest::Assert(!ApoapseAddress::ServerDomain::IsValid(".apspace"));
} UNIT_TEST_END

UNIT_TEST("ApoapseAddressObj")
{
	UnitTest::Assert(ApoapseAddress("apoapse.space:9f86d081884c7d659a2feaa0c55ad015a3bf4f1b2b0b822cd15d6c15b0f00a08").IsValid());
} UNIT_TEST_END

UNIT_TEST("ApoapseAddressObjWrongValue")
{
	UnitTest::Assert(!ApoapseAddress("apoapsespace:9f86d081884c7d659a2feaa0c55ad015a3bf4f1b2b0b822cd15d6c15b0f00a08").IsValid());
} UNIT_TEST_END

#endif	// UNIT_TESTS