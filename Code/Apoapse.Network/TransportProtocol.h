#pragma once
#include "Apoapse.Core\ByteUtils.h"

#define HEADER_LENGTH 5

enum class NetMessageType
{
	unknown,
	command,
	message,
	attachement,
	error,
	info
};

class TransportProtocol
{
public:
	static const unsigned short int headerLength = HEADER_LENGTH;

	void ParseRawHeader(const byte* rawHeader[HEADER_LENGTH], UInt32& outputContentExpectedSize, NetMessageType& outputMessageType)
	{

	}
private:

};