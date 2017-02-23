#pragma once
#include <boost/optional.hpp>
#include "Uuid.h"
#include "ApoapseServer.h"
class LocalUser;
class GenericConnection;

enum class OperationDirection : UInt8
{
	SENT,
	RECEIVED
};

class ApoapseOperation
{
	const OperationDirection m_direction;
	boost::optional<DbId> m_operationId;
	boost::optional<DbId> m_itemId;
	const string m_name;

protected:
	const ApoapseServer& server;
	const string& m_itemDBTableName;
	const Uuid m_uuid;

public:
	struct DBInfo
	{
		DbId operationId;
		string timestamp;	// #TODO Put timestamp into a Datetime type
		OperationDirection direction;
		DbId associatedUserId;
	};

	ApoapseOperation(const Uuid& uuid, const string& name, const string& itemDbTableName, OperationDirection dir, ApoapseServer& serverRef);
	OperationDirection GetDirection() const;

	void SaveToDatabase(const LocalUser& associatedUser, GenericConnection& originConnection);
	void SetItemDbId(DbId id);
	DbId GetItemDbId() const;
	//void SaveToDatabase(const std::vector<LocalUser&> associatedUsers, GenericConnection& originConnection);

	static const DBInfo GetOperationInfoFromDatabase(DbId itemId, const string& operationName, ApoapseServer& server);

protected:
	//************************************
	// Method:    ApoapseOperation::IsIemRegistered - Return true if the current item is registered on the database
	// Access:    public 
	// Returns:   bool
	//************************************
	bool IsIemRegistered();

	virtual void SaveToDatabaseInternal() = 0;

private:
	void LogOperation(DbId associatedUserId);

};