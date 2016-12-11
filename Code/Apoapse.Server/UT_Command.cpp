#include "stdafx.h"

#ifdef UNIT_TESTS

#include "Common.h"
#include "Apoapse.UnitTest/TestsManager.h"
#include "Command.h"

UNIT_TEST("CommandSystem/ReadCommandNameFromRaw")
{
	string testStr1 = u8"CONNECT1\n{\"username\": \"User\",	\"password\" : \"MyPassword\"}";
	string testStr2 = u8"CONNECT2\nGuillaume MyPassword";

	UnitTest::Assert(Command::ReadCommandNameFromRaw(testStr1) == "CONNECT1" && Command::ReadCommandNameFromRaw(testStr2) == "CONNECT2");
} UNIT_TEST_END

#endif	// UNIT_TESTS