#include "stdafx.h"
#include "GenericConnection.h"
#include "Common.h"
#include "CommandsManager.h"
#include "StringExtensions.h"

GenericConnection::GenericConnection(boost::asio::io_service& io_service)
	: TCPConnection(io_service),
	m_readStreamBuffer(COMMAND_BODY_RECEIVE_BUFFER_SIZE)
{

}

GenericConnection::~GenericConnection()
{
	LOG_DEBUG_FUNCTION_NAME();
}

void GenericConnection::SetAssociatedActor(std::shared_ptr<Actor> actor)
{
	ASSERT(!HasAssociatedActor());

	m_associatedActor = actor;
}

bool GenericConnection::HasAssociatedActor() const
{
	return m_associatedActor.is_initialized();
}

bool GenericConnection::OnConnectedToServer()
{
	ListenForCommand();

	return true;
}

bool GenericConnection::OnReceivedError(const boost::system::error_code& error)
{
	LOG << LogSeverity::error << __FUNCTION__ << ": " << error.message();

	// #TODO Handle errors
	return true;
}

void GenericConnection::ListenForCommand()
{
	ASSERT(m_readStreamBuffer.size() == 0);
	ReadUntil(m_readStreamBuffer, '\n', [this](size_t bytesTransferred) { OnReceivedCommandName(bytesTransferred); });
}

void GenericConnection::OnCommandBodyComplete(std::unique_ptr<Command>& command)
{
	global->threadPool->PushTask([this, &command]
	{
		command->ParseRawCmdBody();

		if (command->IsValid())
		{
			global->threadPool->PushTask([this, &command]	// #TODO use a specific thread pool -> do it inside Command::ProcessFromNetwork?
			{
				if (HasAssociatedActor())
					GetAssociatedActor()->ProcessCommandFromNetwork(*command.get());
				else
					command->ProcessFromNetwork(this);

				m_commands.pop_front();	// use GenericConnection::PopLastCommand (used to have a mutex) instead?
			});
			// #TODO support from network and user (do from generic connection in another method that automatically decide if this has to be done on which actor?) Maybe have an actor Interface/base class?
			// This class having an actor base class in a optional shared ptr? This base class just acting as a proxy, the inherited virtual functions should do most (if not all of the operations)

			//TODO: if no errors from ProcessFromNetwork, remove the command from the queue
		}
		else
		{
			// #TODO_NETWORK_ERR_HANDLING
			LOG << "Command invalid, closing connection" << LogSeverity::error;
			Close();
		}
	});

	// #TODO Implement payload receiving system here
	ListenForCommand();
}

void GenericConnection::OnReceivedCommandName(size_t bytesTransferred)
{
	constexpr UInt8 newLineCharacterSize = 1;	// Used to remove the \n at the end of the command name
	const auto data = m_readStreamBuffer.data();
	m_readStreamBuffer.consume(bytesTransferred);

	const string commandName = string(boost::asio::buffers_begin(data), boost::asio::buffers_begin(data) + bytesTransferred - newLineCharacterSize);

	if (CommandsManager::GetInstance().CommandExist(commandName))
	{
		m_commands.push_front(CommandsManager::GetInstance().CreateCommand(commandName));

		auto& currentCommand = m_commands.front();

		if (CheckActorAndCommandCompatibility(*currentCommand.get()))
		{
			ReadCommandFirstChar(*currentCommand.get());
			ListenForCommandBody();
		}
		else
		{
			// #TODO_NETWORK_ERR_HANDLING
			LOG << "The command " << currentCommand->GetConfig().name << " does not support the input from this connection, user, or remote server" << LogSeverity::error;

			m_commands.pop_front();
		}
	}
	else
	{
		LOG << "The command requested from " << GetEndpoint().address() << " does not exist" << LogSeverity::error;
		// #TODO_NETWORK_ERR_HANDLING
	}

	if (m_readStreamBuffer.size() > 0)
		m_readStreamBuffer.consume(m_readStreamBuffer.size());

	ListenForCommand();
}

void GenericConnection::ReadCommandFirstChar(Command& command)
{
	auto data = m_readStreamBuffer.data();
	m_readStreamBuffer.consume(1);

	const string firstChar = string(boost::asio::buffers_begin(data), boost::asio::buffers_begin(data) + 1);

	if (firstChar == "{")
		command.SetInputRealFormat(Format::JSON);
	else
		command.SetInputRealFormat(Format::INLINE);

	command.AppendCommandBodyData(firstChar);
}

void GenericConnection::ListenForCommandBody()
{
	auto& command = m_commands.front();
	const size_t bufferDataSize = m_readStreamBuffer.size();
	const Format commandRealFormat = command->GetInputRealFormat();

	if (bufferDataSize > 0)
	{
		auto data = m_readStreamBuffer.data();
		m_readStreamBuffer.consume(bufferDataSize);
		auto bufferData = string(boost::asio::buffers_begin(data), boost::asio::buffers_begin(data) + bufferDataSize);

		command->AppendCommandBodyData(bufferData);

		// In case the whole command body fit inside the receive stream buffer
		if (commandRealFormat == Format::INLINE)
		{
			if (StringExtensions::get_last_chars(bufferData, 1) == "\n")
			{
				OnCommandBodyComplete(command);
				return;
			}
		}
		else
		{
			if (StringExtensions::get_last_chars(bufferData, 3) == "}\n\n")
			{
				OnCommandBodyComplete(command);
				return;
			}
		}
	}

	if (commandRealFormat == Format::INLINE)
		ReadUntil(m_readStreamBuffer, '\n', [this](size_t bytesTransferred) { OnReceivedCommandInfoBody(bytesTransferred); });
	else
		ReadUntil(m_readStreamBuffer, "}\n\n", [this](size_t bytesTransferred) { OnReceivedCommandInfoBody(bytesTransferred); });
}

void GenericConnection::OnReceivedCommandInfoBody(size_t bytesTransferred)
{
	ASSERT_MSG(m_commands.size() > 0, "The command queue is empty");

	if (bytesTransferred == 0)	// If the delimiter is not found (when bytesTransferred equal 0), call ListenForCommandInfoBody() which will extract the data from the buffer and call the async network operation agin
	{
		ListenForCommandBody();
		return;
	}

	ASSERT(bytesTransferred == m_readStreamBuffer.size());
	auto& command = m_commands.front();

	auto data = m_readStreamBuffer.data();
	m_readStreamBuffer.consume(bytesTransferred);

	command->AppendCommandBodyData(string(boost::asio::buffers_begin(data), boost::asio::buffers_begin(data) + bytesTransferred));

	OnCommandBodyComplete(command);
}

bool GenericConnection::CheckActorAndCommandCompatibility(Command& command)
{
	if (HasAssociatedActor())
		return GetAssociatedActor()->IsCommandCompatibleWithCurrentActor(command);
	else
		return command.CanProcessThisActor(this);
}

// void GenericConnection::PopLastCommand()
// {
// 
// }
