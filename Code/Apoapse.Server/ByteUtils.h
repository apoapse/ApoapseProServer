#pragma once
typedef unsigned char byte;

struct ByteConverter
{
	inline static UInt32 ToUInt32(const byte bytes[], const size_t offset = 0)	// #WARNING: current implementation support little endian only
	{
		return *((UInt32*)(bytes + offset));
	}

	inline static std::string ToASCIIString(const byte bytes[], const size_t length, const size_t offset = 0)
	{
		return std::string((char*)bytes + offset, length);
	}
};