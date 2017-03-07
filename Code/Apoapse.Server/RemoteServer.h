#pragma once
#include "GenericConnection.h"
#include "RemoteServersManager.h"
#include "ApoapseAddress.h"
#include <queue>

class RemoteServer final : public GenericConnection
{
public:
	enum class Direction : UInt8
	{
		CURRENT_SERVER_CONNECTED_TO_REMOTE,
		REMOTE_CONNECTED_TO_CURRENT_SERVER
	};

private:
	ApoapseAddress::ServerDomain m_domain;
	Direction m_direction;
	std::queue<std::unique_ptr<Command>> m_commandsTobeSent;
	bool m_authenticated;
	// #TODO Add activity index and auto-disconnect

public:
	RemoteServer(boost::asio::io_service& io_service, Direction direction, ApoapseServer& apoapseServer);
	virtual ~RemoteServer();

	void ConnectToRemoteServer(const ApoapseAddress::ServerDomain& domain);

	ApoapseAddress::ServerDomain GetServerDomain() const;
	void EnqueCommandToBeSend(std::unique_ptr<Command> command);
	void Authenticate(const ApoapseAddress::ServerDomain& domain);
	bool IsAuthenticated() const;

protected:
	bool CheckCommandNetworkInputCompatibility(Command& command) override;
	void ProcessCommandFromNetwork(Command& command) override;

private:
	void OnConnected() override;
	void SendServerInfoCommand();
	void FlushAndSendCommands();
};