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
	bool OnSentPacket(const boost::system::error_code& error, size_t bytesTransferred);

	inline void decode_utf8(const std::string& bytes, std::wstring& wstr)
	{
		//utf8::utf8to32(bytes.begin(), bytes.end(), std::back_inserter(wstr));
	}
};