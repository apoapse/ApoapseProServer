#include "stdafx.h"
#include "Common.h"
#include "TestConnection.h"

#include "UTF8.h"

TestConnection::TestConnection(boost::asio::io_service& io_service) : TCPConnection(io_service)
{

}

TestConnection::~TestConnection()
{
	Log("TestConnection::~TestConnection");
}

bool TestConnection::OnAcceptedByServer()
{
	Log("TestConnection::OnAcceptedByServer", LogSeverity::debug);
	return true;
}

bool TestConnection::OnConnectedToServer(const boost::system::error_code& error)
{
	if (!error)
	{
		Log("TestConnection::OnConnectedToServer connected to/from port " + std::to_string(GetEndpoint().port()));

		string msg = "FROM SERVER!";
		std::wstring wideMsg = UTF8::utf8_to_wstring(msg);


	
		//byte* p = reinterpret_cast<byte*>(&wideMsg[0]);

		//Send(wideMsg);
		return true;
	}
	else
	{
		Log("TestConnection::OnConnectedToServer " + error.message(), LogSeverity::error);
		return false;
	}
}

bool TestConnection::OnReceivedPacket(std::shared_ptr<NetMessage> netMessage)
{
	std::wstring unicodeMsg = netMessage->GetDataStr();

	Log(Format("%1% (length: %3%) %2%", __FUNCTION__, UTF8::wstring_to_u8string(unicodeMsg), unicodeMsg.length()));
	Send("From server: " + UTF8::wstring_to_u8string(unicodeMsg));
	return true;
}

bool TestConnection::OnReadError(const boost::system::error_code& error)
{
	Log("TestConnection::OnReadError " + error.message(), LogSeverity::error);
	return false;
}

bool TestConnection::OnSentPacket(const std::shared_ptr<NetMessage> netMessage, size_t bytesTransferred)
{
	return true;
}