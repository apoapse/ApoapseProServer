#pragma once
#include "Apoapse.Server/SQLValue.h"
#include "Apoapse.Server/Diagnostics.h"
#include <vector>
#include <boost/optional.hpp>

class SQLRow
{
	std::vector<SQLValue> m_values;

public:
	SQLRow(size_t expectedColumnCount = 0)
	{
		if (expectedColumnCount > 0)
			m_values.reserve(expectedColumnCount);
	}

	void AddValue(const SQLValue& value)
	{
		m_values.push_back(value);
	}

	const SQLValue& operator[](size_t index) const
	{
		ASSERT(ColumnCount() > 0);

		return m_values.at(index);
	}

	size_t ColumnCount() const
	{
		return m_values.size();
	}

	// For range based loop
	std::vector<SQLValue>::const_iterator begin() const
	{
		return m_values.begin();
	}

	std::vector<SQLValue>::const_iterator end() const
	{
		return m_values.end();
	}
};

class SQLResultContainer
{
	boost::optional<std::vector<SQLRow>> m_rows;
	bool m_isSucessfull;

public:
	SQLResultContainer() : m_isSucessfull(false)
	{
	}

	SQLResultContainer(bool isSucessfull) : m_isSucessfull(isSucessfull)
	{
		if (isSucessfull)
			m_rows = std::vector<SQLRow>();	// Initialize the vector for boost::optional
	}

	void AddRow(const SQLRow& row)
	{
		m_rows->push_back(row);
	}

	const SQLRow& operator[](size_t rowIndex) const
	{
		ASSERT(RowCount() > 0);

		return m_rows->at(rowIndex);
	}

	explicit operator bool() const
	{
		return m_isSucessfull;
	}

	size_t RowCount() const
	{
		return (m_rows.is_initialized()) ? m_rows->size() : 0;
	}
};