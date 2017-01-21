#pragma once
#include "Apoapse.Server/Types.h"
#include "Apoapse.Server/Database.h"
#include <boost/config.hpp>
#include <sqlite/sqlite3.h>

class SQLite : public Database
{
	sqlite3* m_database;

public:
	~SQLite();

	bool Open(const char** params, size_t paramsCount) override;
	void Close() override;

	SQLResultContainer ExecQuery(const char* preparedQuery, const SQLValue** values, size_t valuesCount) override;
	const char* GetLastError() override;
};

extern "C" BOOST_SYMBOL_EXPORT SQLite apoapse_internal_dll;
SQLite apoapse_internal_dll;