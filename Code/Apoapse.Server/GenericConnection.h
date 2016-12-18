#pragma once
#include "TCPConnection.h"
#include "Command.h"
#include <boost/optional.hpp>

class GenericConnection : public TCPConnection
{
	boost::asio::streambuf m_readStreamBuffer;
	boost::optional<std::unique_ptr<Command>> m_currentCommand;//TODO

public:
	GenericConnection(boost::asio::io_service& io_service);
	virtual ~GenericConnection();
	
private:


protected:
	virtual bool OnConnectedToServer() override;
	virtual bool OnReceivedError(const boost::system::error_code& error) override;

	virtual void ListenForCommand();
/*	virtual void ListenForCommandPayload();
	virtual void SendCommand(const Command& command);*/
	virtual void OnReceivedCommandName(size_t bytesTransferred);
};