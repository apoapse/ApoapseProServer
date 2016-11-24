#include "stdafx.h"

#ifdef UNIT_TESTS

#include "Common.h"
#include "ByteUtils.h"
#include "Apoapse.UnitTest/TestsManager.h"

UNIT_TEST("ByteConverter/ConvertUInt32")
{
	byte bytes[] = { 188, 3, 0, 0 };

	UnitTest::Assert(ByteConverter::ToUInt32(bytes) == 956);
} UNIT_TEST_END

UNIT_TEST("ByteConverter/ConvertUInt32WithOffset")
{
	byte bytes[] = { 0, 188, 3, 0, 0 };

	UnitTest::Assert(ByteConverter::ToUInt32(bytes, 1) == 956);
} UNIT_TEST_END

UNIT_TEST("ByteConverter/ConvertASCIIString")
{
	byte bytes[] = { 69, 113, 33, 57 };
	const std::string value = ByteConverter::ToASCIIString(bytes, 4);

	UnitTest::Assert(value == "Eq!9");
} UNIT_TEST_END

UNIT_TEST("ByteConverter/ConvertASCIIStringWithOffset")
{
	byte bytes[] = { 69, 113, 33, 57 };
	const std::string value = ByteConverter::ToASCIIString(bytes, 2, 1);

	UnitTest::Assert(value == "q!");
} UNIT_TEST_END

UNIT_TEST("ByteConverter/ConvertUInt32ToBytes")
{
	byte bytes[sizeof(UInt32)];	// 4 bytes
	ByteConverter::FromUInt32(bytes, 29626497);

	UnitTest::Assert(ByteConverter::ToUInt32(bytes) == 29626497);

} UNIT_TEST_END

UNIT_TEST("ByteConverter/ConvertASCIIStringToBytes")
{
	string text = "test conversion";
	byte bytes[15];

	ByteConverter::FromASCIIString(bytes, text);

	UnitTest::Assert(ByteConverter::ToASCIIString(bytes, 15) == text);

} UNIT_TEST_END

#endif	// UNIT_TESTS