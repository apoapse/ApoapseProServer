#include "stdafx.h"
#include "Common.h"
#include "CommandsManager.h"
#include "ApoapseErrorCode.h"

class Error final : public Command
{
public:
	const CommandConfig& GetConfig() override
	{
		static auto config = CommandConfig();
		config.name = "ERROR";
		config.expectedFormat = Format::INLINE;
		config.fields =
		{
			CommmandField{ "error_code", true }
		};

		return config;
	}

	bool PostValidate() const override
	{
		return true;
	}
};

APOAPSE_COMMAND_REGISTER(Error, "ERROR");