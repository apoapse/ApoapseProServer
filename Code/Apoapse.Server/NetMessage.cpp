#include "stdafx.h"
#include "Common.h"
#include "NetMessage.h"
#include "UnicodeUtils.h"
#include "TCPConnection.h"

UInt64 NetMessage::maxAllowedSize = 0;

NetMessage::NetMessage(UInt32 expectedContentSize, bool containApoapseTCPHeader) : m_expectedSize(expectedContentSize), m_containApoapseTCPHeader(containApoapseTCPHeader)
{
	ASSERT_MSG(maxAllowedSize > 0, "NetMessage::SetMaxAllowedSize should have been called");

	if (expectedContentSize > maxAllowedSize)
		LOG << LogSeverity::error << "NetMessage::NetMessage the expected size is higher than the allowed max size";	// #TODO throw a real error and handle it on TCPConnection side

	m_data.reserve(expectedContentSize);

	LOG_DEBUG_ONLY("created a new network message with an expected size of " << m_expectedSize);
}

NetMessage::~NetMessage()
{
	LOG_DEBUG_FUNCTION_NAME();
}

void NetMessage::SetMaxAllowedSize()
{
	ASSERT_MSG(maxAllowedSize == 0, "NetMessage::SetMaxAllowedSize() has already been called");

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

string NetMessage::GetDataU8Str(size_t offset /*= 0*/) const
{
	if (m_containApoapseTCPHeader)
		offset += 4;

	return string(m_data.begin() + offset, m_data.end());
}

bool NetMessage::IsComplete() const
{
	return (GetContentSize() == m_expectedSize);
}

size_t NetMessage::GetContentSize() const
{
	return (m_containApoapseTCPHeader ? (m_data.size() - 255) : m_data.size());
}