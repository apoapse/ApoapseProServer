#include "stdafx.h"
#include "Common.h"
#include "CommandsManager.h"
#include "ApoapseError.h"
#include "RemoteServer.h"

class Error final : public Command
{
public:
	const CommandConfig& GetConfig() override
	{
		static auto config = CommandConfig();
		config.name = "ERROR";
		//config.processFromUser = PROCESS_METHOD_FROM_USER(Error::FromInternal);
		config.processFromRemoteServer = PROCESS_METHOD(RemoteServer, Error::FromExternal);
		config.expectedFormat = Format::INLINE;
		config.fields =
		{
			CommandField{ "error_code", FieldRequirement::VALUE_MENDATORY, FIELD_VALUE_CHECK_TYPE(UInt16) },
			CommandField{ "related_item", FieldRequirement::ANY_OPTIONAL }
		};

		return config;
	}

	bool PostValidate() override
	{
		return true;
	}

private:
// 	bool FromInternal(LocalUser& user, ClientConnection& originConnection)
// 	{
// 		return true;
// 	}

	void FromExternal(RemoteServer& remoteServer)
	{
		LOG << "Received error " << ReadFieldValue<string>("error_code").get() << " from remote server " << remoteServer.GetServerDomain().GetStr() << LogSeverity::warning;

		// #TODO 
	}
};

APOAPSE_COMMAND_REGISTER(Error, "ERROR");