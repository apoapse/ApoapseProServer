#pragma once
#include "Apoapse.Server/Types.h"
class SQLValue;
class SQLResultContainer;

struct Database
{
	virtual bool Open(const char** params, size_t paramsCount) = 0;
	virtual void Close() = 0;

	virtual SQLResultContainer ExecQuery(const char* preparedQuery, const SQLValue** values, size_t valuesCount) = 0;
	virtual const char* GetLastError() = 0;
};