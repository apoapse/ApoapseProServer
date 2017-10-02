#include "stdafx.h"
#include "ServerConnection.h"
#include "Common.h"
#include "ApoapseServer.h"
#include "SecurityAlert.h"

ServerConnection::ServerConnection(boost::asio::io_service& ioService, ApoapseServer* server)
	: GenericConnection(ioService)
	, m_server(server)
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

void ServerConnection::Authenticate(const User::Address& address)
{
	ASSERT(!m_relatedUser.has_value());

	if (m_server->usersManager->IsUserConnected(address))
	{
		// User already connected
		m_relatedUser = m_server->usersManager->GetUserByAddress(address).lock()->GetObjectShared();
		m_relatedUser.value()->AddConnection(this);
	}
	else
	{
		// New user
		m_relatedUser = std::make_shared<User>(address, this, m_server);
		m_server->usersManager->AddConnectedUser(m_relatedUser->get());
	}
}

bool ServerConnection::OnConnectedToServer()
{
	return true;
}

void ServerConnection::OnReceivedValidCommand(std::unique_ptr<Command> cmd)
{
	const bool authenticated = IsAuthenticated();

	try
	{
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
	}
	catch (const std::exception& e)
	{
		LOG << LogSeverity::error << "Exception trigged while processing a command of type " << static_cast<UInt16>(cmd->GetInfo().command) << ": " << e;
		Close();
	}
}
