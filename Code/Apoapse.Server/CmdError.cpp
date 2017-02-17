#include "stdafx.h"
#include "Common.h"
#include "CommandsManager.h"
#include "ApoapseError.h"

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
			CommandField{ "error_code", FieldRequirement::VALUE_MENDATORY, FIELD_VALUE_CHECK_TYPE(UInt16) },
			CommandField{ "related_item", FieldRequirement::ANY_OPTIONAL }
		};

		return config;
	}

	bool PostValidate() const override
	{
		return true;
	}
};

APOAPSE_COMMAND_REGISTER(Error, "ERROR");