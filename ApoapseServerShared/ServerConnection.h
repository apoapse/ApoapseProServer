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

	ServerConnection(boost::asio::io_service& ioService, ApoapseServer* server, ssl::context& context);
	virtual ~ServerConnection() override;

	bool IsAuthenticated() const;
	void Authenticate(const Username& username);
	User* GetRelatedUser() const;
	
private:
	bool OnConnectedToServer() override;
	void OnReceivedValidCommand(CommandV2& cmd) override;
	

};