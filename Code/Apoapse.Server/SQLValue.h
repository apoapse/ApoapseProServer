#pragma once
#include <boost\variant.hpp>
#include <boost\numeric\conversion\cast.hpp>
#include <string>

enum class ValueType
{
	UNSUPPORTED,
	TEXT,
	INT,
	INT_64
};

class SQLValue
{
	boost::variant<std::string, int, Int64> m_data;
	ValueType m_type;

public:
	template <typename T>
	SQLValue(const T& value, ValueType type)
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
		if (typeid(T) == typeid(std::string))
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
			return boost::get<int>(m_data);

		else if (GetType() == ValueType::INT_64)
			return boost::numeric_cast<int>(boost::get<Int64>(m_data));

		else
			throw std::bad_typeid();
	}

	int GetInt64() const
	{
		if (GetType() == ValueType::INT_64)
			return boost::get<Int64>(m_data);

		else if (GetType() == ValueType::INT)
			return static_cast<Int64>(GetInt32());

		else
			throw std::bad_typeid();
	}

	string GetText() const
	{
		if (GetType() == ValueType::TEXT)
			return boost::get<std::string>(m_data);
		else
			throw std::bad_typeid();
	}

	bool GetBoolean() const
	{
		if (GetType() == ValueType::INT_64 || GetType() == ValueType::INT)
			return (GetInt32() == 1);
		else
			throw std::bad_typeid();
	}
};