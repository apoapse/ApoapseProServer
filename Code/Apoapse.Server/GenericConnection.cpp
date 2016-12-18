#include "stdafx.h"
#include "GenericConnection.h"
#include "Common.h"
#include "CommandsManager.h"

GenericConnection::GenericConnection(boost::asio::io_service& io_service)
	: TCPConnection(io_service),
	m_readStreamBuffer(1024)
{

}

GenericConnection::~GenericConnection()
{
	LOG_DEBUG_FUNCTION_NAME();
}

bool GenericConnection::OnConnectedToServer()
{
	ListenForCommand();

	return true;
}

bool GenericConnection::OnReceivedError(const boost::system::error_code& error)
{
	LOG << LogSeverity::error << __FUNCTION__ << ":" << error;

	// #TODO Handle errors
	return true;
}


void GenericConnection::ListenForCommand()
{
	ReadUntil(m_readStreamBuffer, '\n', [this](size_t bytesTransferred) { OnReceivedCommandName(bytesTransferred); });
}
/*
void GenericConnection::ListenForCommandPayload()
{

}

void GenericConnection::SendCommand(const Command& command)
{

}
*/
void GenericConnection::OnReceivedCommandName(size_t bytesTransferred)
{
	constexpr UInt8 newLineCharacterSize = 1;	// Used to remove the \n at the end of the command name

	auto data = m_readStreamBuffer.data();
	m_readStreamBuffer.consume(bytesTransferred);

	const string commandName = string(boost::asio::buffers_begin(data), boost::asio::buffers_begin(data) + bytesTransferred - newLineCharacterSize);
	LOG << commandName;
	if (CommandsManager::GetInstance().CommandExist(commandName))
	{
		m_currentCommand = CommandsManager::GetInstance().CreateCommand(commandName);
	}
	else
	{
		LOG << __FUNCTION__ << ": The command requested from " << GetEndpoint().address() << " does not exist" << LogSeverity::error;

		// #TODO handle error network side
		ListenForCommand();
	}
}

