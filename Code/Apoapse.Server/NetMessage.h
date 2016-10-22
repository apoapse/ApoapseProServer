#pragma once
#include <vector>
#include "ByteUtils.h"
#include <array>

using data_ptr = std::shared_ptr<std::vector<byte>>;

class NetMessage
{
	data_ptr m_data = std::make_shared<std::vector<byte>>();
	UInt32 m_expectedSize = 0;
	static UInt64 maxAllowedSize;

public:
	NetMessage(UInt32 expectedContentSize);
	virtual ~NetMessage();
	static void SetMaxAllowedSize();

	template<typename T, size_t SIZE>
	void AppendData(const std::array<T, SIZE>& remoteData, size_t dataRealSize)
	{
		// #TODO throw a real errors and handle them on TCPConnection side
		ASSERT(!IsComplete());
		ASSERT((m_data->size() + dataRealSize) <= m_expectedSize);

		m_data->insert(m_data->end(), remoteData.begin(), remoteData.end() - (remoteData.size() - dataRealSize));
	}

	data_ptr GetData() const;
	std::wstring GetDataStr() const;
	bool IsComplete() const;
};