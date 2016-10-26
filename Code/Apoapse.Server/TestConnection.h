#pragma once
#include "TCPConnection.h"

class TestConnection : public TCPConnection
{
public:
	TestConnection(boost::asio::io_service& io_service);
	~TestConnection();

private:
	bool OnAcceptedByServer();
	bool OnConnectedToServer(const boost::system::error_code& error);
	bool OnReceivedPacket(std::shared_ptr<NetMessage> netMessage);
	bool OnReadError(const boost::system::error_code& error);
	bool OnSentPacket(const std::shared_ptr<NetMessage> netMessage, size_t bytesTransferred);
};