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

	TEST_METHOD(ReadConfigVariable_string)
	{
		wchar_t message[200];
		const string defaultValue = "default_string_text";
		SettingsManager* settingsManager = SettingsManager::Create();

		settingsManager->Init("test_config.json");
		settingsManager->RegisterConfigVar<string>("test_values.test_string", defaultValue);

		string readValue = settingsManager->ReadConfigValue<string>("test_values.test_string");

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

	TEST_METHOD(ReadConfigVariable_int)
	{
		wchar_t message[200];
		const int defaultValue = 121;
		SettingsManager* settingsManager = SettingsManager::Create();

		settingsManager->Init("test_config.json");
		settingsManager->RegisterConfigVar<int>("test_values.test_int", defaultValue);

		int readValue = settingsManager->ReadConfigValue<int>("test_values.test_int");

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

	TEST_METHOD(ReadConfigVariable_uint)
	{
		wchar_t message[200];
		const unsigned int defaultValue = 991;
		SettingsManager* settingsManager = SettingsManager::Create();

		settingsManager->Init("test_config.json");
		settingsManager->RegisterConfigVar<unsigned int>("test_values.test_uint", defaultValue);

		unsigned int readValue = settingsManager->ReadConfigValue<unsigned int>("test_values.test_uint");

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
};