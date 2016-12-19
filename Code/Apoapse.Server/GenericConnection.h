#pragma once
#include "TCPConnection.h"
#include "Command.h"
#include <deque>

#define COMMAND_BODY_RECEIVE_BUFFER_SIZE 512

class GenericConnection : public TCPConnection
{
	boost::asio::streambuf m_readStreamBuffer;
	std::deque<std::unique_ptr<Command>> m_commands;

public:
	GenericConnection(boost::asio::io_service& io_service);
	virtual ~GenericConnection();
	
private:


protected:
	virtual bool OnConnectedToServer() override;
	virtual bool OnReceivedError(const boost::system::error_code& error) override;

	virtual void ListenForCommand();

	virtual void OnReceivedCommandName(size_t bytesTransferred);
	virtual void ReadCommandFirstChar(const std::unique_ptr<Command>& command);
	virtual void ListenForCommandBody();
	virtual void OnReceivedCommandInfoBody(size_t bytesTransferred);
	virtual void OnCommandBodyComplete(const std::unique_ptr<Command>& command);
};