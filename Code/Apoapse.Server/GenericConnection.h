#pragma once
#include "TCPConnection.h"
#include "Command.h"
#include <deque>
#include <boost/optional.hpp>

#define COMMAND_BODY_RECEIVE_BUFFER_SIZE 512

class GenericConnection : public TCPConnection
{
	boost::asio::streambuf m_readStreamBuffer;
	std::deque<std::unique_ptr<Command>> m_commands;	// #TODO Make sure there are no issues with thread safety

public:
	GenericConnection(boost::asio::io_service& io_service);
	virtual ~GenericConnection();

private:

protected:
	virtual bool OnConnectedToServer() override;
	virtual bool OnReceivedError(const boost::system::error_code& error) override;
	virtual void OnCommandBodyComplete(std::unique_ptr<Command>& command);

	virtual void ListenForCommand();

	virtual void OnReceivedCommandName(size_t bytesTransferred);
	virtual void ReadCommandFirstChar(Command& command);
	virtual void ListenForCommandBody();
	virtual void OnReceivedCommandInfoBody(size_t bytesTransferred);

	virtual bool CheckCommandNetworkInputCompatibility(Command& command) = 0;
	virtual void ProcessCommandFromNetwork(Command& command) = 0;

	//virtual void PopLastCommand();
};