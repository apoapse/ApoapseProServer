#pragma once
#include "Types.h"
#include <string>

typedef unsigned char byte;

struct ByteConverter
{
	static_assert(sizeof(UInt32) == 4, "UInt32 does not use 4 bytes on this system");

	inline static UInt32 ToUInt32(const byte bytes[], const size_t offset = 0)	// #WARNING: current implementation support little-endian only
	{
		return *((UInt32*)(bytes + offset));
	}

	inline static std::string ToASCIIString(const byte bytes[], const size_t length, const size_t offset = 0)
	{
		return std::string((char*)bytes + offset, length);
	}

	static void FromUInt32(byte* output, const UInt32 value)	// #WARNING: current implementation support little-endian only
	{
		ASSERT(sizeof(output) >= sizeof(UInt32));

		memcpy(output, (byte*)&value, sizeof(UInt32));
	}

	static void FromASCIIString(byte* output, const std::string& strvalue)
	{
		strncpy((char*)output, strvalue.c_str(), strvalue.length());
	}
};