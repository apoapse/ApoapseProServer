#include "stdafx.h"
#include "Common.h"
#include "NetMessage.h"
#include "UTF8.h"

UInt64 NetMessage::maxAllowedSize = 0;

NetMessage::NetMessage(UInt32 expectedContentSize) : m_expectedSize(expectedContentSize)
{
	ASSERT_MSG(maxAllowedSize > 0, "NetMessage::SetMaxAllowedSize should have been called");

	if (expectedContentSize > maxAllowedSize)
		Log("NetMessage::NetMessage the expected size is higher than the allowed max size", LogSeverity::error);	// #TODO throw a real error and handle it on TCPConnection side

	m_data->reserve(expectedContentSize);

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

data_ptr NetMessage::GetData() const
{
	return m_data;
}

std::wstring NetMessage::GetDataStr() const
{
	return UTF8::utf8_to_wstring(string(m_data->begin(), m_data->end()));
}

bool NetMessage::IsComplete() const
{
	return (m_data->size() == m_expectedSize);
}