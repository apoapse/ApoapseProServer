#pragma once
#include <boost\any.hpp>
#include <string>

enum class ValueType
{
	UNSUPPORTED,
	TEXT,
	TEXT_STR_OBJ,	// Used only for data from the database
	INT,
	INT_64//ADD SUPPORT OF: NULL & BLOB
};

class SQLValue
{
	boost::any m_data;
	ValueType m_type;

public:
	SQLValue(const boost::any& value, ValueType type)
		: m_data(value),
		m_type(type)
	{
	}

	ValueType GetType() const
	{
		return m_type;
	}

	template <typename T>
	static ValueType GenerateType()
	{
		if (typeid(T) == typeid(string))
			return ValueType::TEXT;

		else if (typeid(T) == typeid(int))
			return ValueType::INT;

		else if (typeid(T) == typeid(Int64))
			return ValueType::INT_64;

		else
			return ValueType::UNSUPPORTED;
	}

	int GetInt32() const
	{
		if (GetType() == ValueType::INT)
			return boost::any_cast<int>(m_data);

		else if (GetType() == ValueType::INT_64)
			return (int)GetInt64();

		else
			throw std::bad_typeid();
	}

	int GetInt64() const
	{
		if (GetType() == ValueType::INT_64)
			return boost::any_cast<Int64>(m_data);

		else if (GetType() == ValueType::INT)
			return static_cast<Int64>(GetInt32());

		else
			throw std::bad_typeid();
	}

	const char* GetText() const
	{
		if (GetType() == ValueType::TEXT)
			return boost::any_cast<const char*>(m_data);
		else
			throw std::bad_typeid();
	}

	string GetTextStr() const
	{
		if (GetType() == ValueType::TEXT_STR_OBJ)
			return boost::any_cast<std::string>(m_data);

		else if (GetType() == ValueType::TEXT)
			return std::string(GetText());

		else
			throw std::bad_typeid();
	}
};