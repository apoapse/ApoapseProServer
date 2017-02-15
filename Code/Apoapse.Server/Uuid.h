#pragma once

class Uuid
{
public:
	static string Generate();
	static bool IsValid(const string& uuidStr);
};