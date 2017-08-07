#pragma once
#include "GenericConnection.h"
class ApoapseServer;

class ServerConnection : public GenericConnection
{
	ApoapseServer* m_server;

public:
	ServerConnection(boost::asio::io_service& ioService, ApoapseServer* server);
	virtual ~ServerConnection() override;
	
private:
	bool OnConnectedToServer() override;
	void OnReceivedPayload(std::shared_ptr<NetworkPayload> payload) override;


};