#include "stdafx.h"

#ifdef UNIT_TESTS
#include "Common.h"
#include "CommandsManager.h"
#include "ApoapseError.h"

class CmdUnitTest final : public Command
{
public:
	const CommandConfig& GetConfig() override
	{
		static auto config = CommandConfig();
		config.name = "UNIT_TEST";
		config.expectedFormat = Format::INLINE;
		config.fields =
		{
			CommandField{ "test_uint16", FieldRequirement::VALUE_MENDATORY, FIELD_VALUE_VALIDATOR(UInt16, CmdUnitTest::ValidateField1) },
			CommandField{ "test_string", FieldRequirement::VALUE_MENDATORY }
		};

		return config;
	}

	bool PostValidate() const override
	{
		return true;
	}

private:

	static bool ValidateField1(UInt16 value)
	{
		return (value > 5 && value < 9999);
	}
};

APOAPSE_COMMAND_REGISTER(CmdUnitTest, "UNIT_TEST");
#endif	// UNIT_TESTS