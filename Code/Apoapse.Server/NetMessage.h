#pragma once
#include <vector>
#include "ByteUtils.h"
#include <array>

class NetMessage
{
	std::vector<byte> m_data;
	const UInt32 m_expectedSize = 0;
	static UInt64 maxAllowedSize;
	const bool m_containApoapseTCPHeader;

public:
	NetMessage(UInt32 expectedContentSize, bool containApoapseTCPHeader);
	virtual ~NetMessage();
	static void SetMaxAllowedSize();

	template<size_t SIZE>
	void AppendData(const std::array<byte, SIZE>& remoteData, size_t dataRealSize)
	{
		// #TODO throw a real errors and handle them on TCPConnection side
		ASSERT(!IsComplete());
		ASSERT((m_data.size() + dataRealSize) <= m_expectedSize);

		m_data.insert(m_data.end(), remoteData.begin(), remoteData.end() - (remoteData.size() - dataRealSize));
	}
	void AppendStr(const std::string& str);

	const std::vector<byte>& GetRawData() const;
	std::wstring GetDataStr(size_t offset = 0) const;
	bool IsComplete() const;
};