#include "stdafx.h"
#include "GenericConnection.h"
#include "Common.h"
#include "CommandsManager.h"
#include "StringExtensions.h"

GenericConnection::GenericConnection(boost::asio::io_service& io_service, ApoapseServer& apoapseServer)
	: TCPConnection(io_service),
	server(apoapseServer),
	m_readStreamBuffer(COMMAND_BODY_RECEIVE_BUFFER_SIZE)
{
}

GenericConnection::~GenericConnection()
{
	LOG_DEBUG_FUNCTION_NAME();
}

void GenericConnection::ReadPayload(std::unique_ptr<Command>& command)
{
	if (command->ActualPayloadSize() < command->ExpectedPayloadSize())
		ReadSome(m_payloadReadBuffer, [this](size_t bytesTransferred) { OnReceivedPayloadData(bytesTransferred); });
	else
		OnCommandReadyForProcessing(command);
}

bool GenericConnection::OnConnectedToServer()
{
	ListenForCommand();
	OnConnected();

	return true;
}

bool GenericConnection::OnReceivedError(const boost::system::error_code& error)
{
	LOG << LogSeverity::error << __FUNCTION__ << ": " << error.message() << " (from: " << GetEndpoint() << ")";
	
	//auto errorType = (boost::asio::error::basic_errors)error.value();

	// #TODO Handle errors
	// http://www.boost.org/doc/libs/1_44_0/doc/html/boost_asio/reference/error__basic_errors.html
	return false;
}

void GenericConnection::ListenForCommand()
{
	ASSERT(m_readStreamBuffer.size() == 0);
	ReadUntil(m_readStreamBuffer, '\n', [this](size_t bytesTransferred) { OnReceivedCommandName(bytesTransferred); });
}

void GenericConnection::OnCommandBodyComplete(std::unique_ptr<Command>& command)
{
	command->ParseRawCmdBody();

	if (command->IsValid())
	{
		if (command->ExpectedPayloadSize() > 0)
		{
			LOG << "Listening for a payload of " << command->ExpectedPayloadSize() << " bytes" << LogSeverity::debug;

			const size_t bufferDataSize = m_readStreamBuffer.size();

			if (bufferDataSize > 0)
			{
				// In the case where the generic stream buffer has data associated with the payload
				auto data = m_readStreamBuffer.data();
				command->AppendPayloadData(std::vector<byte>(boost::asio::buffers_begin(data), boost::asio::buffers_end(data)).data(), bufferDataSize);

				m_readStreamBuffer.consume(bufferDataSize);
			}

			ReadPayload(command);
		}
		else
		{
			ASSERT(m_readStreamBuffer.size() == 0);
			OnCommandReadyForProcessing(command);
		}
	}
	else
	{
		ClearReadBuffer();

		LOG << "Malformed command (" << command->GetConfig().name << ") received from " << GetEndpoint() << LogSeverity::warning;
		ApoapseError::SendError(ApoapseErrorCode::MALFORMED_CMD, *this);
		m_commands.pop_front();
		ListenForCommand();
	}
}

void GenericConnection::OnCommandReadyForProcessing(std::unique_ptr<Command>& command)
{
	ProcessCommandFromNetwork(*command.get());

	m_commands.pop_front();	// use GenericConnection::PopLastCommand (used to have a mutex) instead? YES

	ASSERT(m_readStreamBuffer.size() == 0);
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
		LOG_DEBUG << "Creating command " << currentCommand->GetConfig().name;

		if (CheckCommandNetworkInputCompatibility(*currentCommand.get()))
		{
			ReadCommandFirstChar(*currentCommand.get());
			ListenForCommandBody(bytesTransferred);
		}
		else
		{
			LOG << "The command " << currentCommand->GetConfig().name << " requested from " << GetEndpoint() << " does not support the input from this type of connection" << LogSeverity::error;
			ApoapseError::SendError(ApoapseErrorCode::UNAUTHORIZED_ACTION, *this);

			m_commands.pop_front();
			ClearReadBuffer();
			Close();
		}
	}
	else
	{
		LOG << "The command requested from " << GetEndpoint() << " does not exist" << LogSeverity::warning;
		ApoapseError::SendError(ApoapseErrorCode::MALFORMED_CMD, *this);

		ClearReadBuffer();
		Close();
	}
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

void GenericConnection::ListenForCommandBody(size_t bytesTransferred)
{
	auto& command = m_commands.front();
	const size_t realBufferSize = m_readStreamBuffer.size();
	const size_t bufferDataSize = (bytesTransferred > realBufferSize || bytesTransferred == 0) ? realBufferSize : bytesTransferred;
	
	const Format commandRealFormat = command->GetInputRealFormat();

	if (bufferDataSize > 0)
	{
		auto data = m_readStreamBuffer.data();
		m_readStreamBuffer.consume(bufferDataSize);

		const auto bufferData = string(boost::asio::buffers_begin(data), boost::asio::buffers_begin(data) + bufferDataSize);

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

	ListenForCommandBody(bytesTransferred);
}

void GenericConnection::OnReceivedPayloadData(size_t bytesTransferred)
{
	auto& command = m_commands.front();
	command->AppendPayloadData(m_payloadReadBuffer.data(), bytesTransferred);

	ReadPayload(command);
}

void GenericConnection::ClearReadBuffer()
{
	if (m_readStreamBuffer.size() > 0)
			m_readStreamBuffer.consume(m_readStreamBuffer.size());
}
