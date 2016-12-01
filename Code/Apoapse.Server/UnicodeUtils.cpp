#include "stdafx.h"
#include "UnicodeUtils.h"
#include <codecvt>

using namespace Unicode;
//http://stackoverflow.com/questions/31302506/stdu32string-conversion-to-from-stdstring-and-stdu16string
// #TODO #IMPORTANT

string u16_to_u8string(const std::u16string& str)
{
	//std::wstring_convert<std::codecvt_utf8_utf16<char16_t>, char16_t> converter;
	//return converter.to_bytes(str);
	return string();
}

std::u16string u8string_to_u16(const string& str)
{
	//std::wstring_convert<std::codecvt_utf8_utf16<char16_t>, char16_t> converter;
	//return converter.from_bytes(str);
	return std::u16string();
}