// ----------------------------------------------------------------------------
// Copyright (C) 2020 Apoapse
// Copyright (C) 2020 Guillaume Puyal
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at http://www.apache.org/licenses/LICENSE-2.0
//
// For more information visit https://github.com/apoapse/
// And https://apoapse.space/
// ----------------------------------------------------------------------------

#include "stdafx.h"
#include "Common.h"
#include "Command.h"
#include "CmdNotifyNewUser.h"
#include "CommandsManager.h"
#include "ServerConnection.h"
#include "UsersManager.h"

CommandInfo& CmdNotifyNewUser::GetInfo() const
{
	static auto info = CommandInfo();
	info.command = CommandId::notify_new_user;
	info.clientOnly = true;
	info.requireAuthentication = true;
	info.metadataTypes = MetadataAcess::all;

	return info;
}

void CmdNotifyNewUser::SendNotifyNewUser(INetworkSender& destination, const Username& username, const ApoapseMetadata& metadataAll)
{
	MessagePackSerializer ser;
	ser.UnorderedAppend("username", username.GetRaw());
	ser.UnorderedAppend("metadata_all", metadataAll.GetRawData());

	CmdNotifyNewUser cmd;
	cmd.Send(ser, destination);
}
/*
void CmdNotifyNewUser::SendFromDatabase(DbId id, INetworkSender& connection)
{
	SQLQuery query(*global->database);
	query << SELECT << "username_hash, metadata_all" << FROM << "users" << WHERE << "user_id" << EQUALS << id;
	auto res = query.Exec();
	
	const auto username = Username(res[0][0].GetByteArray());
	const auto metadataAll = ApoapseMetadata(res[0][1].GetByteArray(), MetadataAcess::all);

	SendNotifyNewUser(connection, username, metadataAll);
}*/

//REGISTER_OPERATION_OBJECT(CmdNotifyNewUser, OperationType::new_user);