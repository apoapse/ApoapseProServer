#include "stdafx.h"
#include "Common.h"
#include "TestConnection.h"

#include "UTF8.h"

TestConnection::TestConnection(boost::asio::io_service& io_service) : TCPConnection(io_service)
{

}

TestConnection::~TestConnection()
{
	LOG_DEBUG_FUNCTION_NAME();
}

bool TestConnection::OnAcceptedByServer()
{
	LOG_DEBUG_FUNCTION_NAME();
	return true;
}

bool TestConnection::OnConnectedToServer(const boost::system::error_code& error)
{
	if (!error)
	{
		LOG << __FUNCTION__ << " connected to/from port " << GetEndpoint().port();

		string msg = "FROM SERVER!";
		std::wstring wideMsg = UTF8::utf8_to_wstring(msg);

		return true;
	}
	else
	{
		LOG << LogSeverity::error << __FUNCTION__ << " " << error.message();
		return false;
	}
}

bool TestConnection::OnReceivedPacket(std::shared_ptr<NetMessage> netMessage)
{
	std::wstring unicodeMsg = netMessage->GetDataStr();

	LOG << __FUNCTION__ << "(length: " << unicodeMsg.length() << " " << UTF8::wstring_to_u8string(unicodeMsg);
	Send("From server: " + UTF8::wstring_to_u8string(unicodeMsg));
	return true;
}

bool TestConnection::OnReadError(const boost::system::error_code& error)
{
	LOG << LogSeverity::error << "TestConnection::OnReadError " << error.message();
	return false;
}

bool TestConnection::OnSentPacket(const std::shared_ptr<NetMessage> netMessage, size_t /*bytesTransferred*/)
{
	return true;
}