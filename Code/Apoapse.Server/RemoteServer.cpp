#include "stdafx.h"
#include "RemoteServer.h"
#include "Common.h"
#include "ApoapseServer.h"

RemoteServer::RemoteServer(boost::asio::io_service& io_service, Direction direction, ApoapseServer& apoapseServer)
	: GenericConnection(io_service, apoapseServer)
	, m_direction(direction)
	, m_authenticated(false)
{
}

RemoteServer::~RemoteServer()
{
	server.GetRemoteServersManager().RemoveConnectedServer(this);

	if (!m_commandsTobeSent.empty())
	{
		// #TODO 
	}
}

void RemoteServer::ConnectToRemoteServer(const ApoapseAddress::ServerDomain& domain)
{
	ASSERT(m_direction == Direction::CURRENT_SERVER_CONNECTED_TO_REMOTE);

	m_domain = domain;
	m_authenticated = true;

	Connect("127.0.0.1", 3008); // #TODO Get info from DNS
}

bool RemoteServer::CheckCommandNetworkInputCompatibility(Command& command)
{
	return command.CanProcessFrom(this);
}

void RemoteServer::ProcessCommandFromNetwork(Command& command)
{
	command.ProcessFromNetwork(this);
}

void RemoteServer::OnConnected()
{
	if (m_direction == Direction::CURRENT_SERVER_CONNECTED_TO_REMOTE)
	{
		SendServerInfoCommand();
		LOG << "Connected to remote server " << GetEndpoint();
	}

	server.GetRemoteServersManager().AddConnectedServer(this);

	if (IsAuthenticated())
		FlushAndSendCommands();
}

ApoapseAddress::ServerDomain RemoteServer::GetServerDomain() const
{
	return m_domain;
}

void RemoteServer::SendServerInfoCommand()
{
	ASSERT_MSG(m_direction == Direction::CURRENT_SERVER_CONNECTED_TO_REMOTE, "It's the role of the connected server to say who he is");

	std::unique_ptr<Command> command = CommandsManager::GetInstance().CreateCommand("SERVER_INFO");
	command->InsertFieldValue<string>("domain", m_domain.GetStr());

	command->Send(*this);
}

void RemoteServer::EnqueCommandToBeSend(std::unique_ptr<Command> command)
{
	m_commandsTobeSent.push(std::move(command));

	if (IsConnected())
		FlushAndSendCommands();
}

void RemoteServer::FlushAndSendCommands()
{
	while (!m_commandsTobeSent.empty())
	{
		auto& cmd = m_commandsTobeSent.front();
		cmd->Send(*this);

		m_commandsTobeSent.pop();
	}
}

bool RemoteServer::IsAuthenticated() const
{
	return m_authenticated;
}

void RemoteServer::Authenticate(const ApoapseAddress::ServerDomain& domain)
{
	ASSERT(m_direction == Direction::REMOTE_CONNECTED_TO_CURRENT_SERVER);

	m_domain = domain;
	m_authenticated = true;

	LOG << "Remote server " << GetEndpoint() << " connected and authenticated";

	FlushAndSendCommands();
}
