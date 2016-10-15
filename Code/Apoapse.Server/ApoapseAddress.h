#pragma once
#include <string>

struct ApoapseAddress
{
	static const size_t FULL_ADDRESS_MAX_LENGTH = 255;

	ApoapseAddress(const std::string& rawAddress);

	bool IsValid() const;
	std::string GetFullAddress() const;
	std::string GetDomain() const;
	std::string GetUsername() const;

	bool operator== (const ApoapseAddress& otherObj);

	static ApoapseAddress* Create(const std::string& rawAddress)
	{
		return new ApoapseAddress(rawAddress);
	}

private:
	bool m_preValidated;
	std::string m_domain;
	std::string m_username;
};