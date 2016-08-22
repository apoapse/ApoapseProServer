#include "stdafx.h"
#include "CppUnitTest.h"
#include "Apoapse.Core\SettingsManager.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

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

	TEST_METHOD(RegisterAndRead_string)
	{
		wchar_t message[200];
		const string defaultValue = "default_string_text";
		SettingsManager* settingsManager = SettingsManager::Create();

		settingsManager->Init("test_config.json");
		settingsManager->RegisterConfigVar_string("test_values.test_string", defaultValue);

		const string readValue = settingsManager->ReadConfigValue_string("test_values.test_string");

		if (readValue != "text")
		{
			if (readValue == defaultValue)
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

	TEST_METHOD(RegisterAndRead_int)
	{
		wchar_t message[200];
		const int defaultValue = 121;
		SettingsManager* settingsManager = SettingsManager::Create();

		settingsManager->Init("test_config.json");
		settingsManager->RegisterConfigVar_int("test_values.test_int", defaultValue);

		int readValue = settingsManager->ReadConfigValue_int("test_values.test_int");

		if (readValue != -10)
		{
			if (readValue == defaultValue)
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

	TEST_METHOD(RegisterAndRead_uint)
	{
		wchar_t message[200];
		const unsigned int defaultValue = 991;
		SettingsManager* settingsManager = SettingsManager::Create();

		settingsManager->Init("test_config.json");
		settingsManager->RegisterConfigVar_uint("test_values.test_uint", defaultValue);

		const unsigned int readValue = settingsManager->ReadConfigValue_uint("test_values.test_uint");

		if (readValue != 80)
		{
			if (readValue == defaultValue)
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

	TEST_METHOD(RegisterAndRead_bool)
	{
		wchar_t message[200];
		const bool defaultValue = true;
		SettingsManager* settingsManager = SettingsManager::Create();

		settingsManager->Init("test_config.json");
		settingsManager->RegisterConfigVar_bool("test_values.test_bool", defaultValue);

		const bool readValue = settingsManager->ReadConfigValue_bool("test_values.test_bool");

		if (readValue != false)
		{
			if (readValue == defaultValue)
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