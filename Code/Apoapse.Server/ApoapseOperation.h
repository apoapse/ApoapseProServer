#pragma once
#include <boost/optional.hpp>
#include "Uuid.h"
#include "ApoapseServer.h"
#include "ApoapseAddress.h"
class LocalUser;
class GenericConnection;
class Command;

enum class OperationDirection : UInt8
{
	UNDEFINDED,
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
	ApoapseServer& server;
	const string& m_itemDBTableName;
	const Uuid m_uuid;

public:
	struct DBInfo
	{
		DbId operationId;
		Int64 timestamp;
		OperationDirection direction;
		DbId associatedUserId;
	};

	ApoapseOperation(const Uuid& uuid, const string& name, const string& itemDbTableName, OperationDirection dir, ApoapseServer& serverRef);
	OperationDirection GetDirection() const;

	void SaveToDatabase(GenericConnection& originConnection);
	void LogOperation(DbId associatedUserId, OperationDirection direction = OperationDirection::SENT);
	void SetItemDbId(DbId id);
	DbId GetItemDbId() const;
	void Send(const std::vector<ApoapseAddress>& recipients, GenericConnection& originConnection);

	Uuid GetItemUuid() const;

	static const DBInfo GetOperationInfoFromDatabase(DbId itemId, const string& operationName, ApoapseServer& server);
	static OperationDirection ReadDirectionFromDatabase(const string& rawDbDirectionText);

	virtual std::unique_ptr<Command> PrepareCommandToBeSent() = 0;

protected:
	//************************************
	// Method:    ApoapseOperation::IsIemRegistered - Return true if the current item is registered on the database
	// Access:    public 
	// Returns:   bool
	//************************************
	bool IsIemRegistered();

	virtual void SaveToDatabaseInternal() = 0;
};