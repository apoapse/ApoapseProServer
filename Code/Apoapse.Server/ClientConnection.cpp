#include "stdafx.h"
#include "ClientConnection.h"
#include "Common.h"
#include "LocalUser.h"
#include "ApoapseServer.h"

ClientConnection::ClientConnection(boost::asio::io_service& io_service, ApoapseServer& apoapseServer) : GenericConnection(io_service, apoapseServer)
{

}

ClientConnection::~ClientConnection()
{
	if (HasAssociatedUser())
		GetAssociatedUser()->RemoveAssociatedConnection(this);
}

bool ClientConnection::HasAssociatedUser() const
{
	return m_associatedUser.is_initialized();
}

std::shared_ptr<LocalUser> ClientConnection::GetAssociatedUser() const
{
	ASSERT(HasAssociatedUser());

	return m_associatedUser.get();
}

void ClientConnection::SetAssociatedUser(std::shared_ptr<LocalUser> user)
{
	ASSERT_MSG(!HasAssociatedUser(), "An user is already linked to this client connection");

	user->AssociateConnection(this);
	m_associatedUser = user;
}

bool ClientConnection::CheckCommandNetworkInputCompatibility(Command& command)
{
	if (HasAssociatedUser())
		return command.CanProcessFrom(GetAssociatedUser().get());
	else
		return command.CanProcessFrom(this);
}

void ClientConnection::ProcessCommandFromNetwork(Command& command)
{
	if (HasAssociatedUser())
		command.ProcessFromNetwork(GetAssociatedUser().get());
	else
		command.ProcessFromNetwork(this);
}
