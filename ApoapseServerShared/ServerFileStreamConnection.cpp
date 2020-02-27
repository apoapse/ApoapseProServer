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
#include "ServerFileStreamConnection.h"
#include <boost/asio/ssl/context.hpp>
#include "ApoapseServer.h"
#include <filesystem>
#include "UsersManager.h"
#include "ServerConnection.h"
#include "ApoapseOperation.h"
#include "CommandV2.h"
#include "UsersManager.h"

/*
Attachment::Attachment(DataStructure& data)
{
	uuid = data.GetField("uuid").GetValue<Uuid>();
	parentThread = data.GetField("parent_thread").GetValue<Uuid>();
	fileName = data.GetField("name").GetValue<std::string>();
	fileSize = data.GetField("file_size").GetValue<Int64>();
}*/

ServerFileStreamConnection::ServerFileStreamConnection(boost::asio::io_service& ioService, ApoapseServer* server, boost::asio::ssl::context& context)
	: FileStreamConnection(ioService, context)
	, server(*server)
{
}

ServerConnection* ServerFileStreamConnection::GetMainConnection() const
{
	return m_mainConnection;
}

void ServerFileStreamConnection::SetMainConnection(ServerConnection* serverConnection)
{
	m_mainConnection = serverConnection;
}

void ServerFileStreamConnection::ErrorDisconnectAll()
{
	LOG_DEBUG << "ErrorDisconnectAll";

	if (m_mainConnection && m_mainConnection->IsConnected())
		GetMainConnection()->Close();
	
	Close();
}

void ServerFileStreamConnection::Authenticate(const Username& username, const hash_SHA256& authCode)
{
	global->mainThread->PushTask([this, username, authCode]()
	{
		if (server.usersManager->IsUserConnected(username))
		{
			auto user = server.usersManager->GetUserByUsername(username);
			user.lock()->AuthenticateFileStream(authCode, this);
		}
		else
		{
			LOG << LogSeverity::error << "Unable to authenticate as the user is not connected";
		}
	});
}

void ServerFileStreamConnection::OnSocketConnected()
{
}

void ServerFileStreamConnection::OnFileDownloadCompleted(const AttachmentFile& file)
{
	LOG_DEBUG << "OnFileDownloadCompleted " << file.fileName;

	global->mainThread->PushTask([file, this]()
	{
		auto dat = global->apoapseData->ReadItemFromDatabase("attachment", "uuid", file.uuid);
		dat.GetField("is_downloaded").SetValue(true);
		dat.SaveToDatabase();

		const Uuid parentMsg = dat.GetField("parent_message").GetValue<Uuid>();
		DataStructure relatedMessage = global->apoapseData->ReadItemFromDatabase("message", "uuid", parentMsg);
		
		DataStructure cmdDat = global->apoapseData->GetStructure("attachment_uuid");
		cmdDat.GetField("uuid").SetValue(dat.GetField("uuid").GetValue<Uuid>());
		CommandV2 cmd = global->cmdManager->CreateCommand("attachment_available", cmdDat);
		
		if (relatedMessage.GetField("direct_recipient").HasValue())
		{
			const Username recipient = relatedMessage.GetField("direct_recipient").GetValue<Username>();
			
			ApoapseOperation::RegisterOperation("attachment_available", m_mainConnection->GetRelatedUser()->GetUsername(), dat.GetDbId(), OperationOwnership::self);
			ApoapseOperation::RegisterOperation("attachment_available", recipient, dat.GetDbId(), OperationOwnership::self);

			cmd.Send(*m_mainConnection);
			if (server.usersManager->IsUserConnected(recipient))
				cmd.Send(*server.usersManager->GetUserByUsername(recipient).lock());
		}
		else
		{
			ApoapseOperation::RegisterOperation("attachment_available", m_mainConnection->GetRelatedUser()->GetUsername(), dat.GetDbId(), OperationOwnership::all);
			cmd.Send(*server.usersManager);
		}
	});
}

void ServerFileStreamConnection::OnFileSentSuccessfully(const AttachmentFile& file)
{
	LOG << "File sent successfully " << file.fileName;
}
