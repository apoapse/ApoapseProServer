#pragma once
#include "TCPConnection.h"
#include "Command.h"
#include <deque>
#include <boost/optional.hpp>
#include "ApoapseError.h"
#include <array>
#include "ByteUtils.h"
class ApoapseServer;

#define COMMAND_BODY_RECEIVE_BUFFER_SIZE 255
#define COMMAND_PAYLOAD_RECEIVE_BUFFER_SIZE 512

class GenericConnection : public TCPConnection
{
	boost::asio::streambuf m_readStreamBuffer;
	std::array<byte, COMMAND_PAYLOAD_RECEIVE_BUFFER_SIZE> m_payloadReadBuffer;
	std::deque<std::unique_ptr<Command>> m_commands;	// #TODO Make sure there are no issues with thread safety

public:
	ApoapseServer& server;

	GenericConnection(boost::asio::io_service& io_service, ApoapseServer& apoapseServer);
	virtual ~GenericConnection();

private:
	void ReadPayload(std::unique_ptr<Command>& command);

protected:
	bool OnConnectedToServer() override;
	virtual void OnConnected() = 0;
	virtual bool OnReceivedError(const boost::system::error_code& error) override;
	virtual void OnCommandBodyComplete(std::unique_ptr<Command>& command);
	virtual void OnCommandReadyForProcessing(std::unique_ptr<Command>& command);

	virtual void ListenForCommand();

	virtual void OnReceivedCommandName(size_t bytesTransferred);
	virtual void ReadCommandFirstChar(Command& command);
	virtual void ListenForCommandBody(size_t bytesTransferred);
	virtual void OnReceivedCommandInfoBody(size_t bytesTransferred);
	virtual void OnReceivedPayloadData(size_t bytesTransferred);

	virtual bool CheckCommandNetworkInputCompatibility(Command& command) = 0;
	virtual void ProcessCommandFromNetwork(Command& command) = 0;

	void ClearReadBuffer();
};