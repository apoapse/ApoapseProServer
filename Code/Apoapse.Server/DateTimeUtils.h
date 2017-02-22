#pragma once
#include <boost/date_time/posix_time/posix_time.hpp>
#include <regex>

namespace DateTimeUtils
{
	class UTCDateTime	// ISO 8601
	{
		string m_dateTimeStr;

	public:
		UTCDateTime(const string& dateTime) : m_dateTimeStr(dateTime)
		{
			SECURITY_ASSERT(ValidateFormat(dateTime));
		}

		string str() const
		{
			return m_dateTimeStr;
		}

		bool operator<(const UTCDateTime& foreignDateTime) const
		{
			return (str() < foreignDateTime.str());
		}

		bool operator>(const UTCDateTime& foreignDateTime) const
		{
			return (str() > foreignDateTime.str());
		}

		static UTCDateTime CurrentTime()
		{
			using namespace boost::posix_time;
			ptime t = second_clock::universal_time();

			auto test = to_iso_extended_string(t);

			return UTCDateTime(to_iso_extended_string(t) + "Z");
		}

		static bool ValidateFormat(const string& dateStr)
		{
			if (dateStr.length() != 20)
				return false;

			std::regex expr("[0-9]{4}-[0-1][0-9]-[0-3][1-9]T[0-2][0-9]:[0-5][0-9]:[0-5][0-9]Z");
			return std::regex_match(dateStr, expr);
		}
	};

}