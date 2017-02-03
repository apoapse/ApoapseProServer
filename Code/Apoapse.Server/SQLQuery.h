#pragma once
#include <sstream>
#include "Database.h"
#include "SQLValue.h"
#include "SQLPackagedResult.h"

#define SELECT		"SELECT "
#define ALL			" * "
#define FROM		" FROM "
#define WHERE		" WHERE "
#define EQUALS		" = "
#define OR			" OR "
#define AND			" AND "
#define INSERT_INTO	"INSERT INTO "
#define VALUES		" VALUES "
#define UPDATE		"UPDATE "
#define SET			" SET "

class DBException : public std::exception
{
	const std::string m_errorMsg;

public:
	DBException(std::string errorMsg) : m_errorMsg(std::move(errorMsg))
	{
	}

	virtual const char* what() const override
	{
		return m_errorMsg.c_str();
	}
};

class SQLQuery
{
	Database& m_database;
	std::stringstream m_query;
	std::vector<SQLValue*> m_values;

public:
	SQLQuery(Database& database) : m_database(database)
	{
	}

	virtual ~SQLQuery()
	{
		for (auto* ptr : m_values)
			delete ptr;

		m_values.clear();
	}

	const SQLPackagedResult Exec()
	{
		const string preparedQuery = GetPreparedFullQuery();
		const SQLValue** valuesArray = (m_values.size() > 0) ? (const SQLValue**)m_values.data() : NULL;

		const auto result = m_database.ExecQuery(preparedQuery.c_str(), valuesArray, m_values.size());

		if (!result)
			throw DBException("SQL error on query [" + preparedQuery + "] - " + string(m_database.GetLastError()));

		return result;
	}

	std::future<SQLPackagedResult> ExecAsync()
	{
		return global->threadPool->PushTask([this]
		{
			return Exec();
		});
	}

	string GetPreparedFullQuery() const
	{
		return m_query.str();
	}

	//************************************
	// Method:    SQLQuery::operator<< - For anything that is part of a SQL query syntax
	// // Parameter: const char* queryStatement
	//************************************
	SQLQuery& operator<<(const char* queryStatement)
	{
		m_query << queryStatement;

		return *this;
	}

	//************************************
	// Method:    SQLQuery::operator<< - Handle the input value variables
	// // Parameter: const T & value
	//************************************
	template <typename T>
	SQLQuery& operator<<(const T& value)
	{
		AddValue(new SQLValue(value, SQLValue::GenerateType<T>()));

		return *this;
	}

private:
	void AddValue(SQLValue* sqlValue)	// #TODO Add safety checks (when ENABLE_SEC_ADVANCED_CHECKS is defined)
	{
		m_values.push_back(sqlValue);
		m_query << "?"/* << m_values.size()*/;	// Insert the value placeholder
	}
};