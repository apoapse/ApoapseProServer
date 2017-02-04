#include "stdafx.h"

#ifdef UNIT_TESTS
#include "Common.h"
#include "CommandsManager.h"
#include "ApoapseError.h"

class UnitTestCmd final : public Command
{
public:
	const CommandConfig& GetConfig() override
	{
		static auto config = CommandConfig();
		config.name = "UNIT_TEST";
		config.expectedFormat = Format::INLINE;
		config.fields =
		{
			CommmandField{ "test_uint16", true, FIELD_VALUE_CHECK_TYPE(UInt16) },
			CommmandField{ "test_string", true }
		};

		return config;
	}

	bool PostValidate() const override
	{
		return true;
	}
};

APOAPSE_COMMAND_REGISTER(UnitTestCmd, "UNIT_TEST");
#endif	// UNIT_TESTS