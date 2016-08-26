#include "stdafx.h"
#include "Platforms.h"
#include "StringExtensions.h"
#include <boost\algorithm\string.hpp>

inline size_t StringExtensions::CountOccurences(const std::string& string, char character)
{
	return std::count(string.begin(), string.end(), character);
}

inline void StringExtensions::SplitString(std::vector<std::string>& dataOutput, const std::string& inputStr, const std::string& separator)
{
	boost::split(dataOutput, inputStr, boost::is_any_of(separator), boost::token_compress_on);
}

inline void StringExtensions::EraseCharacters(std::string& inputStr, const std::string& characterToErase)
{
	boost::erase_all(inputStr, characterToErase);
}

inline std::vector<std::string> StringExtensions::RegexMatchAll(const std::regex& regexPattern, const std::string& inputText)
{
	std::vector<std::string> output(std::sregex_token_iterator(inputText.begin(), inputText.end(), regexPattern), std::sregex_token_iterator());

	return output;
}