#include "stdafx.h"
#include "NetMessage.h"
#include "Apoapse.Core\Common.h"

NetMessage::NetMessage(NetMessageType messageType, unsigned int expectedContentSize) : m_type(messageType), m_expectedContentSize(expectedContentSize)
{
	if (expectedContentSize < 1)
		throw "";

	m_data.reserve(expectedContentSize);
}

NetMessage::~NetMessage()
{

}

/*void NetMessage::WriteHeader(const string& value)
{

}*/

void NetMessage::PushData(const boost::array<char, SOCKET_READ_BUFFER_SIZE>& remoteBuffer, size_t dataSize)
{
	size_t totalDataSize = 0;

	for (size_t i = 0; i < remoteBuffer.size(); i++)
	{
		const char currentChar = remoteBuffer.at(i);
		m_data.push_back(currentChar);

		totalDataSize += sizeof(currentChar);
		if (totalDataSize == dataSize)
			break;
	}

	//m_data.insert(m_data.end(), remoteBuffer.begin(), remoteBuffer.end() - (remoteBuffer.size() - dataSize));

	OnPushData();
}

void NetMessage::PushData(const string& inputStr)
{
	m_data.insert(m_data.end(), inputStr.begin(), inputStr.end());

	OnPushData();
}

void NetMessage::OnPushData()
{
	const size_t dataSize = m_data.size();
	if (dataSize > m_expectedContentSize)
		throw "TODO";

#ifdef DEBUG
	if (dataSize == m_expectedContentSize)
		Log(Format("%1% NetMessage full (%2% bytes)", __FUNCTION__, dataSize), LogSeverity::debug);
#endif // DEBUG
}

std::vector<char> NetMessage::GetData() const
{
	return m_data;
}

string NetMessage::GetDataString() const
{
	return string(m_data.begin(), m_data.end());
}

NetMessageType NetMessage::GetMessageType() const
{
	return m_type;
}

NetMessage NetMessage::FromNetRawHeader(const boost::array<char, HEADER_LENGTH>& headerBytes)
{
	NetMessageType messageType;
	/*switch (remoteBuffer.at(0))
	{
	case 'C':
		messageType = NetMessageType::command;
		break;

	case 'M':
		messageType = NetMessageType::message;
		break;

	case 'A':
		messageType = NetMessageType::attachement;
		break;

	case 'E':
		messageType = NetMessageType::error;
		break;

	case 'I':
		messageType = NetMessageType::info;
		break;

	default:
		messageType = NetMessageType::unknown;
		break;
	}

	const string expectedContentSizeStr = string(remoteBuffer.begin() + 1, remoteBuffer.end() - (remoteBuffer.size() - dataLength));
	size_t expectedContentSize = std::stoi(expectedContentSizeStr);*/

	//unsigned int expectedContentSize = 


	//return NetMessage(messageType, expectedContentSize);
	return NetMessage(NetMessageType::unknown, 0);
}

NetMessage NetMessage::FromString(const string& msgContent, NetMessageType messageType)
{
	NetMessage netMessage(messageType, msgContent.size());
	netMessage.PushData(msgContent);

	return netMessage;
}