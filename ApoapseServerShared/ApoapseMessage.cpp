#include "stdafx.h"
#include "ApoapseMessage.h"
#include "Common.h"
#include "SQLQuery.h"

bool ApoapseMessage::DoesMessageExist(const Uuid & uuid)
{
	SQLQuery query(*global->database);
	query << "SELECT Count(*) FROM messages WHERE uuid " << EQUALS << uuid.GetInRawFormat();
	auto res = query.Exec();

	return (res[0][0].GetInt64() == 1);
}

DbId ApoapseMessage::GetMessageDbIdByUuid(const Uuid& uuid)
{
	SQLQuery query(*global->database);
	query << "SELECT id FROM messages WHERE uuid " << EQUALS << uuid.GetInRawFormat();
	auto res = query.Exec();

	return (res.RowCount() == 1) ? res[0][0].GetInt64() : -1;
}

Uuid ApoapseMessage::GetMessageUuidByDbId(DbId dbId)
{
	SQLQuery query(*global->database);
	query << "SELECT uuid FROM messages WHERE id " << EQUALS << dbId;
	auto res = query.Exec();
	
	return Uuid(res[0][0].GetByteArray());
}
