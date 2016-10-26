#include "stdafx.h"
#include "Common.h"
#include "NetMessage.h"
#include "UTF8.h"

UInt64 NetMessage::maxAllowedSize = 0;

NetMessage::NetMessage(UInt32 expectedContentSize, bool containApoapseTCPHeader) : m_expectedSize(expectedContentSize), m_containApoapseTCPHeader(containApoapseTCPHeader)
{
	ASSERT_MSG(maxAllowedSize > 0, "NetMessage::SetMaxAllowedSize should have been called");

	if (expectedContentSize > maxAllowedSize)
		Log("NetMessage::NetMessage the expected size is higher than the allowed max size", LogSeverity::error);	// #TODO throw a real error and handle it on TCPConnection side

	m_data.reserve(expectedContentSize);

	LOG_DEBUG_ONLY(Format("%1% created a new network message with an expected size of %2%", __FUNCTION__, m_expectedSize));
}

NetMessage::~NetMessage()
{
	LOG_DEBUG_ONLY("NetMessage destroyed");
}

void NetMessage::SetMaxAllowedSize()
{
	// Set the max allowed size of any network message by using the attachments max size and adding a margin of 255 bytes for the eventual size of meta data and syntax
	maxAllowedSize = (UInt64)round(global->settings->ReadConfigValue_double("messages.attachments.max_attachement_size_mb")) * 1000000 + 255;
}

void NetMessage::AppendStr(const std::string& str)
{
	//byte const* p = reinterpret_cast<byte const*>(&str[0]);
	//std::size_t size = str.size() * sizeof(str.front());

	//m_data.insert(m_data.end(), p, p + size);
	m_data.insert(m_data.end(), str.begin(), str.end());
}

const std::vector<byte>& NetMessage::GetRawData() const
{
	return m_data;
}

std::wstring NetMessage::GetDataStr(size_t offset) const
{
	if (m_containApoapseTCPHeader)
		offset += 4;

	return UTF8::utf8_to_wstring(string(m_data.begin() + offset, m_data.end()));
}

bool NetMessage::IsComplete() const
{
	return (m_data.size() == m_expectedSize);
}