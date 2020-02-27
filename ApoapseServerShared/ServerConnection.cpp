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
#include "ServerConnection.h"
#include "Common.h"
#include "ApoapseServer.h"
#include "ApoapseError.h"
#include "UsersManager.h"
#include "CommandV2.h"
#include "ServerFileStreamConnection.h"

ServerConnection::ServerConnection(boost::asio::io_service& ioService, ApoapseServer* server, ssl::context& context)
	: GenericConnection(ioService, context)
	, server(*server)
{

}

ServerConnection::~ServerConnection()
{
	if (ServerConnection::IsAuthenticated())
	{
		m_relatedUser.value()->RemoveConnection(this);
	}

	//if (m_fileStream)
	//	m_fileStream->SetMainConnection(nullptr);
}

void ServerConnection::Close() // #TODO FIXME
{
	TCPConnection::Close();

	if (m_fileStream)
		m_fileStream->Close();
}

bool ServerConnection::IsAuthenticated() const
{
	return m_relatedUser.has_value();
}

User& ServerConnection::Authenticate(const Username& username)
{
	ASSERT(!m_relatedUser.has_value());

	if (server.usersManager->IsUserConnected(username))
	{
		// User already connected
		m_relatedUser = server.usersManager->GetUserByUsername(username).lock()->GetObjectShared();
		m_relatedUser.value()->AddConnection(this);
	}
	else
	{
		// New user
		m_relatedUser = server.usersManager->CreateUserObject(username, *this);
		LOG << "User " << username << " connected";
	}

	return *m_relatedUser->get();
}

User* ServerConnection::GetRelatedUser() const
{
	return (m_relatedUser.has_value()) ? m_relatedUser.value().get() : nullptr;
}

std::optional<Username> ServerConnection::GetConnectedUser() const
{
	if (IsAuthenticated())
		return m_relatedUser.value()->GetUsername();
	else
		return std::optional<Username>();
}

void ServerConnection::SetRelatedFileStream(ServerFileStreamConnection* fileStream)
{
	m_fileStream = fileStream;
}

ServerFileStreamConnection* ServerConnection::GetFileStream() const
{
	return m_fileStream;
}

bool ServerConnection::OnConnectedToServer()
{
	return true;
}

void ServerConnection::OnReceivedCommand(CommandV2& cmd)
{
	global->mainThread->PushTask([this, &cmd]()
	{
		if (cmd.IsValid(GetRelatedUser()))
			global->cmdManager->OnReceivedCmdInternal(cmd, *this, GetRelatedUser());
		else
			ApoapseError(ApoapseErrors::invalid_cmd, this);
	});
}
