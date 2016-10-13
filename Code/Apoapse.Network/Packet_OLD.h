#pragma once/*
#include <vector>
#include <string>
#include <boost\array.hpp>

#define SOCKET_READ_BUFFER_SIZE 10	// #TODO: find the most relevant value

class Packet
{
private:
	std::vector<char> m_localBuffer;
	boost::array<char, SOCKET_READ_BUFFER_SIZE>& m_remoteReadBuffer;

public:
	Packet(boost::array<char, SOCKET_READ_BUFFER_SIZE>& buffer) : m_remoteReadBuffer(buffer)
	{
	}

	~Packet()
	{
	}

	void Push(size_t bytesTransferred)
	{
		m_localBuffer.insert(m_localBuffer.end(), m_remoteReadBuffer.begin(), m_remoteReadBuffer.end() - (m_remoteReadBuffer.size() - bytesTransferred));
	}

	size_t GetSize() const
	{
		return m_localBuffer.size();
	}

	std::vector<char> Data() const
	{
		return m_localBuffer;
	}

	std::string DataToString() const
	{
		return std::string(m_localBuffer.begin(), m_localBuffer.end());
	}
};*/