#include "stdafx.h"
#include "CppUnitTest.h"
#include "Apoapse.Core\SettingsManager.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

#define TOSTRING(x) #x

#define TEST_REGISTER_AND_READ(_type, _defaultValue, _expectedValue)\
		TEST_METHOD(RegisterAndRead##_type)\
		{\
			wchar_t message[200];\
			const auto defaultValue = (_defaultValue);\
			SettingsManager* settingsManager = SettingsManager::Create();\
			\
			settingsManager->Init("test_config.json");\
			settingsManager->RegisterConfigVar##_type("test_values.test" TOSTRING(_type), (_defaultValue));\
			\
			const auto readValue = settingsManager->ReadConfigValue##_type("test_values.test" TOSTRING(_type));\
			\
			if (readValue != (_expectedValue))\
			{\
				if (readValue == defaultValue)\
				{\
					_swprintf(message, L"The default value has been used instead of the value from the config file");\
					Assert::Fail(message);\
				}\
				else\
				{\
					Assert::Fail();\
				}\
			}\
		}\


TEST_CLASS(SettingsManagerTests)
{
public:
	TEST_METHOD(LoadAndReadConfigFile)
	{
		SettingsManager* settingsManager = SettingsManager::Create();

		try
		{
			settingsManager->Init("test_config.json");
		}
		catch (const std::exception&)
		{
			Assert::Fail();
		}
	}

	TEST_REGISTER_AND_READ(_string, "default_string_text", "text");
	TEST_REGISTER_AND_READ(_int, 121, -10);
	TEST_REGISTER_AND_READ(_uint, 991, 80);
	TEST_REGISTER_AND_READ(_bool, true, false);

	TEST_METHOD(RegisterAndRead_double)
	{
		wchar_t message[200];
		const double defaultValue = -5.36;
		SettingsManager* settingsManager = SettingsManager::Create();

		settingsManager->Init("test_config.json");
		settingsManager->RegisterConfigVar_double("test_values.test_double", defaultValue);

		const double readValue = settingsManager->ReadConfigValue_double("test_values.test_double");

		if (!(std::fabs(readValue - 31.52) <= std::numeric_limits<double>::epsilon()))
		{
			if ((std::fabs(readValue - defaultValue) <= std::numeric_limits<double>::epsilon()))
			{
				_swprintf(message, L"The default value has been used instead of the value from the config file");
				Assert::Fail(message);
			}
			else
			{
				Assert::Fail();
			}
		}
	}

	TEST_METHOD(RegisterAndReadMultiple)
	{
		SettingsManager* settingsManager = SettingsManager::Create();

		settingsManager->Init("test_config.json");
		settingsManager->RegisterConfigVar_string("test_values.test_string", "default_string_text");
		settingsManager->RegisterConfigVar_int("test_values.test_int", 121);

		const int readValue_int = settingsManager->ReadConfigValue_int("test_values.test_int");
		const string readValue_string = settingsManager->ReadConfigValue_string("test_values.test_string");

		if ((readValue_int != -10) || (readValue_string != "text"))
		{
			Assert::Fail();
		}
	}

	TEST_METHOD(ReadDefaultValue_bool)
	{
		const bool defaultValue = true;
		SettingsManager* settingsManager = SettingsManager::Create();

		settingsManager->Init("test_config.json");
		settingsManager->RegisterConfigVar_bool("test_values.test_bool_DOES_NOT_EXIST", defaultValue);

		const bool readValue = settingsManager->ReadConfigValue_bool("test_values.test_bool_DOES_NOT_EXIST");

		if (readValue != defaultValue)
		{
			Assert::Fail();
		}
	}
};