#include "stdafx.h"
#include "Common.h"
#include "CommandsManager.h"
#include "ClientConnection.h"
#include "LocalUser.h"
#include "ApoapseAddress.h"
#include "ApoapseServer.h"

class CmdConnect final : public Command
{
public:
	CmdConnect() : Command()
	{
	}

	const CommandConfig& GetConfig() override
	{
		static auto config = CommandConfig();
		config.name = "CONNECT";
		config.expectedFormat = Format::INLINE;
		config.processFromClient = PROCESS_METHOD(ClientConnection, CmdConnect::Authenticate);
		config.fields =
		{
			CommandField { "username", FieldRequirement::VALUE_MENDATORY, FIELD_VALUE_VALIDATOR(string, ApoapseAddress::UsernameHash::IsValid) },
			CommandField { "password", FieldRequirement::VALUE_MENDATORY/*, TODO*/ }
		};

		return config;
	}

	bool PostValidate() const override
	{
		return true;
	}

private:
	void Authenticate(ClientConnection& connection)
	{
		const auto usernamehash = ApoapseAddress::UsernameHash(ReadFieldValue<string>("username").get());

		std::shared_ptr<LocalUser> user = connection.server.GetUsersManager().Authenticate(usernamehash); // #TODO Implement password check

		if (user)
			connection.SetAssociatedUser(user);
		else
		{
			LOG << "User " << usernamehash << " tried to sign-in from " << connection.GetEndpoint() << " using a wrong username or password" << LogSeverity::error;
			ApoapseError::SendError(ApoapseErrorCode::WRONG_LOGIN_CREDENTIALS, connection);
		}
	}
};

APOAPSE_COMMAND_REGISTER(CmdConnect, "CONNECT");