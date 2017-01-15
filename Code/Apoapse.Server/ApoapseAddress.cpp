#include "stdafx.h"
#include "ApoapseAddress.h"
#include "Common.h"
#include "StringExtensions.h"

ApoapseAddress::ApoapseAddress(const string& address) : m_fullAddress(address)
{
	ParseAddress();
}

ApoapseAddress::~ApoapseAddress()
{

}

void ApoapseAddress::ParseAddress()
{
	if (m_fullAddress.length() < 5 && m_fullAddress.length() < (UsernameHash::username_hash_str_size + ServerDomain::max_server_domain_length))
		return;

	if (StringExtensions::contains(m_fullAddress, ':'))
	{
		std::vector<string> temp;
		StringExtensions::split(m_fullAddress, temp, ":");

		if (temp.size() == 2)
		{
			string domain = temp.at(0);
			string username = temp.at(1);

			if (ServerDomain::IsValid(domain) && UsernameHash::IsValid(username))
			{
				m_domain = domain;
				m_username = username;

				m_isValid = true;
			}
		}
	}
}

bool ApoapseAddress::IsValid() const
{
	return m_isValid;
}
