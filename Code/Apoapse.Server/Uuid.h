#pragma once

class Uuid
{
	const string m_uuidStr;

public:
	Uuid(const string& uuidStr);
	string GetStr() const;

	static string Generate();
	static bool IsValid(const string& uuidStr);

	friend std::ostream& operator<<(std::ostream& stream, const Uuid& obj)
	{
		stream << obj.GetStr();
		return stream;
	}
};