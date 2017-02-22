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
	const string m_itemDBTableName;
	const Uuid m_uuid;

public:
	ApoapseOperation(const Uuid& uuid, const string& name, const string& itemDbTableName, OperationDirection dir, ApoapseServer& serverRef);
	OperationDirection GetDirection() const;

	void SaveToDatabase(const LocalUser& associatedUser, GenericConnection& originConnection);


	//************************************
	// Method:    ApoapseOperation::IsIemRegistered - Return true if the current item is registered on the database
	// Access:    public 
	// Returns:   bool
	//************************************
	bool IsIemRegistered();

protected:
	virtual DbId SaveToDatabaseInternal() = 0;

private:
	void LogOperation(DbId associatedUserId);

};