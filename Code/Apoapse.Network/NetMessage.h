#pragma once
#include "Apoapse.Core\Platforms.h"
#include <vector>
#include <boost\array.hpp>
#include "TransportProtocol.h"

#define SOCKET_READ_BUFFER_SIZE 10	// #TODO: find the most relevant value



class NetMessage
{
	std::vector<char> m_data;
	const size_t m_expectedContentSize;
	const NetMessageType m_type;

public:
	DLL_EXPORT NetMessage(NetMessageType messageType, unsigned int expectedContentSize);
	DLL_EXPORT ~NetMessage();

	DLL_EXPORT void PushData(const boost::array<char, SOCKET_READ_BUFFER_SIZE>& remoteBuffer, size_t dataSize);
	DLL_EXPORT void PushData(const string& inputStr);
	//DLL_EXPORT string GenerateFullNetMsg() const;	// Generate a full net message, including the header
	DLL_EXPORT std::vector<char> GetData() const;
	DLL_EXPORT string GetDataString() const;
	DLL_EXPORT NetMessageType GetMessageType() const;

	DLL_EXPORT static NetMessage FromNetRawHeader(const boost::array<char, HEADER_LENGTH>& headerBytes);
	DLL_EXPORT static NetMessage FromString(const string& msgContent, NetMessageType messageType);

private:
	void OnPushData();
	//DLL_EXPORT void WriteHeader(const string& value);
	/*static void NetEncrypt(char* input)
	{

	}*/
};