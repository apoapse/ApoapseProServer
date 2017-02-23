#include "stdafx.h"
#include "SQLQuery.h"
#include "Common.h"
#include "StringExtensions.h"

std::vector<string> SQLQuery::TextToArray(const string& str)
{
	std::vector<string> output;

	StringExtensions::split(str, output, "\n");

	return output;
}
