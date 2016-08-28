#include "stdafx.h"
#include "TestConnection.h"
#include "Common.h"

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
		Log("TestConnection::OnConnectedToServer connected to port " + std::to_string(GetEndpoint().port()));
		return true;
	}
	else
	{
		Log("TestConnection::OnConnectedToServer " + error.message(), LogSeverity::error);
		return false;
	}
}

bool TestConnection::OnReceivedPacket(const boost::system::error_code& error, size_t bytesTransferred)
{
	if (!error)
	{
		std::string output(m_readBuffer, m_readBuffer + sizeof(m_readBuffer));

		//	Remove empty chars and C null char from the buffer
		auto pos = output.find_first_of('\r');	//TEST ONLY
		output.resize(pos);

		Log(output);

		return true;
	}
	else
	{
		Log("TestConnection::OnReceivedPacket " + error.message(), LogSeverity::error);
		return false;
	}
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