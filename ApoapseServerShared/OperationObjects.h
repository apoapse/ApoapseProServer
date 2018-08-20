#pragma once
#include "ObjectsFactory.hpp"
#include "Operation.h"
#include "SQLQuery.h"

#define REGISTER_OPERATION_OBJECT(_class, _key)	OBJECTS_FACTORY_REGISTER_2(OperationObjects::GetInstance().operationObjects, IOperationObject, _class, OperationType, _key)

struct IOperationObject
{
	virtual void SendFromDatabase(DbId id, class ServerConnection& connection) = 0;

	virtual ~IOperationObject() = default;
};

class OperationObjects
{
	OperationObjects() = default;

public:
	ObjectsFactory<OperationType, IOperationObject> operationObjects;

	static OperationObjects& GetInstance()
	{
		static OperationObjects operationObjects;
		return operationObjects;
	}

	OperationObjects(OperationObjects const&) = delete;
	void operator=(OperationObjects const&) = delete;

	static void SynchronizeSince(Int64 sinceTimestamp, class ServerConnection& connection);

private:
	std::unique_ptr<IOperationObject> CreateObject(OperationType operation);
	bool DoesObjectExist(OperationType operation) const;
};