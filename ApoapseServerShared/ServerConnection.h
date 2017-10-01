#pragma once
#include "GenericConnection.h"
#include <optional>
#include "User.h"
class ApoapseServer;

class ServerConnection : public GenericConnection
{
	ApoapseServer* m_server;
	std::optional<std::shared_ptr<User>> m_relatedUser;

public:
	ServerConnection(boost::asio::io_service& ioService, ApoapseServer* server);
	virtual ~ServerConnection() override;

	bool IsAuthenticated() const;
	void Authenticate(const User::Address& address);
	
private:
	bool OnConnectedToServer() override;
	void OnReceivedValidCommand(std::unique_ptr<Command> cmd) override;
	

};