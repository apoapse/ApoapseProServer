#pragma once
#include <string>

namespace Unicode
{
	string u16_to_u8string(const std::u16string& str);
	std::u16string u8string_to_u16(const string& str);
}