#include "stdafx.h"
#ifdef UNIT_TESTS

#include "Common.h"
#include "Apoapse.UnitTest/TestsManager.h"
#include "StringExtensions.h"

UNIT_TEST("StringExtensions:erase_left")
{
	string str = "test";
	StringExtensions::erase_left(str, 's');

	UnitTest::Assert(str == "st");
} UNIT_TEST_END

UNIT_TEST("StringExtensions:erase_right")
{
	string str = "abcdefg";
	StringExtensions::erase_right(str, 'd');

	UnitTest::Assert(str == "abcd");
} UNIT_TEST_END

UNIT_TEST("StringExtensions:erase_all")
{
	string str = "tests";
	StringExtensions::erase_all(str, 's');

	UnitTest::Assert(str == "tet");
} UNIT_TEST_END

UNIT_TEST("StringExtensions:trim_left")
{
	string str = " test \n";
	StringExtensions::trim_left(str);

	UnitTest::Assert(str == "test \n");
} UNIT_TEST_END

UNIT_TEST("StringExtensions:trim_right")
{
	string str = " test \n";
	StringExtensions::trim_right(str);

	UnitTest::Assert(str == " test");
} UNIT_TEST_END

UNIT_TEST("StringExtensions:split")
{
	string str = "StringExtensions/split";
	std::vector<string> resultValues;

	StringExtensions::split(str, resultValues, string("/"));

	UnitTest::Assert(resultValues.size() == 2 && resultValues.at(0) == "StringExtensions" && resultValues.at(1) == "split");
} UNIT_TEST_END

UNIT_TEST("StringExtensions:get_last_chars")
{
	string str = "StringExtensions";

	UnitTest::Assert(StringExtensions::get_last_chars(str, 3) == "ons");
} UNIT_TEST_END

UNIT_TEST("StringExtensions:contains")
{
	string str = "String:Extensions";

	UnitTest::Assert(StringExtensions::contains(str, ':'));
} UNIT_TEST_END

#endif	// UNIT_TESTS