#include "stdafx.h"
#include "SettingsManager.h"
#include "ConfigVariable.h"

int main()
{
	auto settingsManager = ApoapseCore::SettingsManager::Create();
	settingsManager.Init("config.json");
	settingsManager.RegisterConfigVar<string>("general.test_string", "AAaA");

	printf(settingsManager.ReadConfigValue<string>("general.test_string").c_str());

	//auto val = settingsManager.ReadConfigValue<int>("dzd");

	string inputstr;
	getline(std::cin, inputstr);

	return 0;
}