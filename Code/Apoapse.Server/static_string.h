#pragma once

class static_string
{
	const char* const m_data;
	const size_t m_size;

public:
	template <size_t SIZE>
	constexpr static_string(const char(&input)[SIZE])
		: m_data(input),
		m_size(SIZE - 1)
	{
	}

	constexpr char operator[](size_t index) const
	{
		return (index < m_size) ? m_data[index] : throw std::out_of_range("");
	}

	constexpr bool operator==(const static_string& other) const
	{
		return (other.c_str() == c_str());
	}

	constexpr size_t size() const
	{
		return m_size;
	}

	constexpr const char* c_str() const
	{
		return m_data;
	}
};