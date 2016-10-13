#pragma once
#include <string>

struct ApoapseAddress
{
	static const size_t FULL_ADDRESS_MAX_LENGTH = 255;

	ApoapseAddress(const std::string& rawAddress);

	DLL_API bool IsValid() const;
	DLL_API std::string GetFullAddress() const;
	DLL_API std::string GetDomain() const;
	DLL_API std::string GetUsername() const;

	DLL_API bool operator== (const ApoapseAddress& otherObj);

	DLL_API static ApoapseAddress* Create(const std::string& rawAddress)
	{
		return new ApoapseAddress(rawAddress);
	}

private:
	bool m_preValidated;
	std::string m_domain;
	std::string m_username;
};