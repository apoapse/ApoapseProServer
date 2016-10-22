#pragma once
#include <string>
#include <codecvt>

namespace UTF8
{
	inline std::wstring utf8_to_wstring(const std::string& str)
	{
		std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
		return converter.from_bytes(str);
	}

	inline std::string wstring_to_u8string(const std::wstring& str)
	{
		std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
		return converter.to_bytes(str);
	}
}