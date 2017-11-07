#include "stdafx.h"
#include "ServerConnection.h"
#include "Common.h"
#include "ApoapseServer.h"
#include "SecurityAlert.h"
#include "UsersManager.h"
#include "UsergroupsManager.h"

ServerConnection::ServerConnection(boost::asio::io_service& ioService, ApoapseServer* server)
	: GenericConnection(ioService)
	, server(*server)
{

}

ServerConnection::~ServerConnection()
{
	if (IsAuthenticated())
	{
		m_relatedUser.value()->RemoveConnection(this);
	}
}

bool ServerConnection::IsAuthenticated() const
{
	return m_relatedUser.has_value();
}

void ServerConnection::Authenticate(const Username& username)
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
	}
}

bool ServerConnection::OnConnectedToServer()
{
	return true;
}

void ServerConnection::OnReceivedValidCommand(std::unique_ptr<Command> cmd)
{
	const bool authenticated = IsAuthenticated();

	if (cmd->GetInfo().clientOnly)
	{
		SecurityLog::LogAlert(ApoapseErrorCode::cannot_processs_cmd_from_this_connection_type, *this);
		return;
	}

#if !DEBUG	// We disable exception handling on the debug build so that we can catch the exceptions with the debugger
 	try
 	{
#endif
		if (cmd->GetInfo().requireAuthentication && authenticated)
		{
			cmd->Process(*m_relatedUser.value(), *this);
		}
		else if (cmd->GetInfo().onlyNonAuthenticated && !authenticated)
		{
			cmd->Process(*this);
		}
		else if (!cmd->GetInfo().requireAuthentication && !authenticated)
		{
			cmd->Process(*this);
		}
		else
		{
			SecurityLog::LogAlert(ApoapseErrorCode::cannot_processs_cmd_from_this_connection_type, *this);
		}

#if !DEBUG
	}
	catch (const std::exception& e)
	{
		LOG << LogSeverity::error << "Exception trigged while processing a command of type " << static_cast<UInt16>(cmd->GetInfo().command) << ": " << e;
		Close();
	}
#endif
}
