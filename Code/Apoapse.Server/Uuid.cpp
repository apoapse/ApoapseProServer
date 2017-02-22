#include "stdafx.h"
#include "Uuid.h"
#include "Common.h"

#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp>
#include "StringExtensions.h"

Uuid::Uuid(const string& uuidStr) : m_uuidStr(uuidStr)
{
	SECURITY_ASSERT(IsValid(uuidStr));
}

string Uuid::GetStr() const
{
	return m_uuidStr;
}

string Uuid::Generate()
{
	static boost::uuids::random_generator generator;
	boost::uuids::uuid uuid = generator();

	string output = boost::uuids::to_string(uuid);
	StringExtensions::erase_all(output, '-');	// Remove the dashes in the hexadecimal representation
	return output;
}

bool Uuid::IsValid(const string& uuidStr)
{
	return (uuidStr.length() == 32 && StringExtensions::IsOnlyAlphanumeric(uuidStr));
}
