#pragma once
#include "GenericConnection.h"
#include <optional>
#include "User.h"
class ApoapseServer;

class ServerConnection : public GenericConnection
{
	std::optional<std::shared_ptr<User>> m_relatedUser;

public:
	ApoapseServer& server;

	ServerConnection(boost::asio::io_service& ioService, ApoapseServer* server);
	virtual ~ServerConnection() override;

	bool IsAuthenticated() const;
	void Authenticate(const User::Username& username);
	
private:
	bool OnConnectedToServer() override;
	void OnReceivedValidCommand(std::unique_ptr<Command> cmd) override;
	

};