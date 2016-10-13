#include "stdafx.h"
#include "CppUnitTest.h"
#include "Apoapse.Core\ByteUtils.h"
#include <string>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

TEST_CLASS(ByteConverterTests)
{
public:
	TEST_METHOD(ConvertUInt32)
	{
		byte bytes[] = { 188, 3, 0, 0 };

		Assert::IsTrue(ByteConverter::ToUInt32(bytes) == 956);
	}

	TEST_METHOD(ConvertUInt32WithOffset)
	{
		byte bytes[] = { 0, 188, 3, 0, 0 };

		Assert::IsTrue(ByteConverter::ToUInt32(bytes, 1) == 956);
	}

	TEST_METHOD(ConvertASCIIString)
	{
		byte bytes[] = { 69, 113, 33, 57 };
		std::string value = ByteConverter::ToASCIIString(bytes, 4);

		Assert::IsTrue(value == "Eq!9");
	}

	TEST_METHOD(ConvertASCIIStringWithOffset)
	{
		byte bytes[] = { 69, 113, 33, 57 };
		std::string value = ByteConverter::ToASCIIString(bytes, 2, 1);

		Assert::IsTrue(value == "q!");
	}
};