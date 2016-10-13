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
	bool OnReceivedPacket(const NetMessage* packet);
	bool OnReadError(const boost::system::error_code& error);
	bool OnSentPacket(const boost::system::error_code& error, size_t bytesTransferred);
};