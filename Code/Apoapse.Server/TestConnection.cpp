#include "stdafx.h"
#include "Common.h"
#include "TestConnection.h"

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
		Send(boost::asio::buffer("SEND TEST"));
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
	Log(Format("%1% %2%", __FUNCTION__, netMessage->GetDataStr()));

	return true;
}

bool TestConnection::OnReadError(const boost::system::error_code& error)
{
	Log("TestConnection::OnReadError " + error.message(), LogSeverity::error);
	return false;
}

bool TestConnection::OnSentPacket(const boost::system::error_code& error, size_t bytesTransferred)
{
	if (error)
	{
		Log("TestConnection::OnSentPacket " + error.message(), LogSeverity::error);
		return false;
	}
	else
		return true;
}