#include "stdafx.h"

#ifdef UNIT_TESTS

#include "SettingsManager.h"
#include "Apoapse.UnitTest/TestsManager.h"

#define TOSTRING(x) #x
#define TEST_REGISTER_AND_READ(_type, _defaultValue, _expectedValue)\
		UNIT_TEST("SettingsManager/RegisterAndRead/" TOSTRING(_type))\
		{\
			const auto defaultValue = (_defaultValue);\
			SettingsManager settingsManager("test_config.json");\
			\
			settingsManager.RegisterConfigVar##_type("test_values.test" TOSTRING(_type), (_defaultValue));\
			\
			const auto readValue = settingsManager.ReadConfigValue##_type("test_values.test" TOSTRING(_type));\
			\
			if (readValue != (_expectedValue))\
			{\
				if (readValue == defaultValue)\
					UnitTest::Fail("The default value has been used instead of the value from the config file");\
				else\
					UnitTest::Fail();\
			}\
			else\
				UnitTest::Success();\
		} UNIT_TEST_END\


UNIT_TEST("SettingsManager/LoadAndReadConfigFile")
{
	

	try
	{
		SettingsManager settingsManager("test_config.json");
	}
	catch (const std::exception& e)
	{
		UnitTest::Fail(e.what());
	}

	UnitTest::Success();
} UNIT_TEST_END

TEST_REGISTER_AND_READ(_string, "default_string_text", "text");
TEST_REGISTER_AND_READ(_int, 121, -10);
TEST_REGISTER_AND_READ(_uint, 991, 80);
TEST_REGISTER_AND_READ(_bool, true, false);

UNIT_TEST("SettingsManager/RegisterAndReadMultiple")
{
	SettingsManager settingsManager("test_config.json");

	settingsManager.RegisterConfigVar_string("test_values.test_string", "default_string_text");
	settingsManager.RegisterConfigVar_int("test_values.test_int", 121);

	const int readValue_int = settingsManager.ReadConfigValue_int("test_values.test_int");
	const string readValue_string = settingsManager.ReadConfigValue_string("test_values.test_string");

	if ((readValue_int != -10) || (readValue_string != "text"))
	{
		UnitTest::Fail();
	}
	else
	{
		UnitTest::Success();
	}
} UNIT_TEST_END

UNIT_TEST("SettingsManager/ReadDefaultValue_bool")
{
	const bool defaultValue = true;	
	SettingsManager settingsManager("test_config.json");

	settingsManager.RegisterConfigVar_bool("test_values.test_bool_DOES_NOT_EXIST", defaultValue);

	const bool readValue = settingsManager.ReadConfigValue_bool("test_values.test_bool_DOES_NOT_EXIST");

	if (readValue != defaultValue)
	{
		UnitTest::Fail();
	}
	else
	{
		UnitTest::Success();
	}
} UNIT_TEST_END

#endif	// UNIT_TESTS