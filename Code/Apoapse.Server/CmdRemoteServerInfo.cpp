#include "stdafx.h"
#include "Common.h"
#include "CommandsManager.h"
#include "ApoapseAddress.h"
#include "RemoteServer.h"

class RemoteServerInto final : public Command
{
public:
	const CommandConfig& GetConfig() override
	{
		static auto config = CommandConfig();
		config.name = "SERVER_INFO";
		config.expectedFormat = Format::JSON;
		config.processFromRemoteServer = PROCESS_METHOD(RemoteServer, RemoteServerInto::RemoteServerAuthenticate)
		config.fields =
		{
			CommandField{ "domain", FieldRequirement::VALUE_MENDATORY, FIELD_VALUE_VALIDATOR(string, ApoapseAddress::ServerDomain::IsValid) }
		};

		return config;
	}

	bool PostValidate() const override
	{
		return true;
	}

private:
	void RemoteServerAuthenticate(RemoteServer& remoteServer)
	{
		remoteServer.Authenticate(ApoapseAddress::ServerDomain(ReadFieldValue<string>("domain").get()));
	}
};

APOAPSE_COMMAND_REGISTER(RemoteServerInto, "SERVER_INFO");