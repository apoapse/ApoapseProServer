#include "stdafx.h"
#include "ApoapseAddress.h"
#include "Common.h"
#include "Platforms.h"
#include <boost\algorithm\string.hpp>
#include "StringExtensions.h"
#include <vector>

ApoapseAddress::ApoapseAddress(const std::string& rawAddress) : m_preValidated(true), m_domain(""), m_username("")
{
	ASSERT_MSG(rawAddress.length() <= FULL_ADDRESS_MAX_LENGTH, "The address length is too high. This must be checked before creating the object");

	if (StringExtensions::CountOccurences(rawAddress, ':') == 1)
	{
		std::vector<string> tmpVector;

		StringExtensions::SplitString(tmpVector, rawAddress, ":");

		if (tmpVector.size() == 2)
		{
			m_domain = tmpVector.at(0);
			m_username = tmpVector.at(1);
		}
		else
			m_preValidated = false;
	}
	else
		m_preValidated = false;
}

bool ApoapseAddress::IsValid() const
{
	if (m_preValidated == false)
		return false;

	return true;	// #TODO implement: verify both domain and username
}

string ApoapseAddress::GetFullAddress() const
{
	ASSERT(m_domain.length() != 0 && m_username.length() != 0);
	return m_domain + ":" + m_username;
}

string ApoapseAddress::GetDomain() const
{
	return m_domain;
}

string ApoapseAddress::GetUsername() const
{
	return m_username;
}

bool ApoapseAddress::operator== (const ApoapseAddress& otherObj)
{
	return (boost::iequals(otherObj.m_username, this->m_username) && boost::iequals(otherObj.m_domain, this->m_domain));
}