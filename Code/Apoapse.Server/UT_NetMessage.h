#pragma once
#include "NetMessage.h"

struct UT_NetMessage
{
	UT_NetMessage()
	{
		/*UNIT_TEST("NetMessage/Create")
		{
			NetMessage::SetMaxAllowedSize();
			NetMessage netMessage(25);

			byte contentPart1[15];
			ByteConverter::FromASCIIString(contentPart1, "Message: part 1");

			byte contentPart2[10];
			ByteConverter::FromASCIIString(contentPart2, "Message p2");

			netMessage.AppendData<byte, 15>(ArrayToSTDArray<byte, 15>(contentPart1), 15);
			netMessage.AppendData<byte, 15>(ArrayToSTDArray<byte, 15>(contentPart2), 10);

			if (!netMessage.IsComplete())
				UnitTest::Fail("NetMessage not complete");

			UnitTest::Assert(netMessage.GetDataStr() == "Message: part 1Message p2");

		} UNIT_TEST_END*/
	}

private:
	template<typename T, size_t SIZE>
	static std::array<T, SIZE> ArrayToSTDArray(const T* inputArray, size_t size = SIZE)	// Ugly, do not use outside this test
	{
		std::array<T, SIZE> outputArray;

		for (size_t i = 0; i < size; i++)
		{
			outputArray.at(i) = inputArray[i];
		}

		return outputArray;
	}
};