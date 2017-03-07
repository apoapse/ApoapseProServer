#pragma once
#include "ByteUtils.h"
#include "StringExtensions.h"
#define SHA256_SIZE_BYTES 32

class ApoapseAddress
{
public:
	class UsernameHash
	{
		string m_data;
		//boost::optional<std::array<byte, SHA256_SIZE_BYTES>> m_TODO;

	public:
		static const UInt8 username_hash_str_size = 64;

		UsernameHash() = default;
		UsernameHash(const string& usernameHash) : m_data(usernameHash)
		{
			SECURITY_ASSERT(IsValid(usernameHash));
		}

		string GetStr() const
		{
			return m_data;
		}

		void operator=(const string& usernameHash)
		{
			SECURITY_ASSERT(IsValid(usernameHash));
			m_data = usernameHash;
		}

		bool operator<(const UsernameHash& usernameHash) const	// Comparaison operator for std::map
		{
			return (usernameHash.GetStr() > m_data);
		}

// 		std::array<byte, SHA256_SIZE_BYTES> GetInBinary() const
// 		{
// 			
// 		}

		bool operator==(const UsernameHash& secondObj)
		{
			return (secondObj.m_data == this->m_data);
		}

		bool operator==(const UsernameHash& secondObj) const
		{
			return (secondObj.m_data == this->m_data);
		}

		static bool IsValid(const string& usernameHash)
		{
			return (usernameHash.length() == username_hash_str_size && StringExtensions::IsOnlyAlphanumeric(usernameHash));
		}

		friend std::ostream& operator<<(std::ostream& stream, const UsernameHash& obj)
		{
			stream << obj.GetStr();
			return stream;
		}
	};

	class ServerDomain
	{
		string m_data;

	public:
		static const UInt8 max_server_domain_length = 126; // = two times 63

		ServerDomain() = default;
		ServerDomain(const string& domain) : m_data(domain)
		{
			SECURITY_ASSERT(IsValid(domain));
		}

		void operator=(const string& domain)
		{
			SECURITY_ASSERT(IsValid(domain));
			m_data = domain;
		}

		bool operator<(const ApoapseAddress::ServerDomain& domain) const	// Comparaison operator for std::map
		{
			return (domain.GetStr() > m_data);
		}

		string GetStr() const
		{
			return m_data;
		}

		bool operator==(const ServerDomain& secondObj)
		{
			return (secondObj.m_data == this->m_data);
		}

		bool operator==(const ServerDomain& secondObj) const
		{
			return (secondObj.m_data == this->m_data);
		}

		static bool IsValid(const string& serverTLD)
		{
			if (serverTLD.length() > max_server_domain_length || serverTLD.length() < 2)
				return false;

			const size_t result = serverTLD.find('.');
			// #TODO Check if there are no specials characters
			if (result != std::string::npos && result != 0 && result != (serverTLD.length() - 1))
				return true;
			else
				return false;
		}
	};

private:
	const string m_fullAddress;
	ServerDomain m_domain;
	UsernameHash m_username;
	bool m_isValid = { false };

public:
	ApoapseAddress(const string& fullAddress);
	ApoapseAddress(const ServerDomain& domain, const UsernameHash& usernameHash);
	virtual ~ApoapseAddress();

	bool IsValid() const;
	string GetFullAddress() const;
	const ServerDomain& GetDomain() const;
	const UsernameHash& GetUsernameHash() const;

	friend std::ostream& operator<<(std::ostream& stream, const ApoapseAddress& obj)
	{
		stream << obj.GetFullAddress();
		return stream;
	}

	bool operator!=(const ApoapseAddress& secondObj) const
	{
		return !(*this == secondObj.GetFullAddress());
	}

	bool operator==(const ApoapseAddress& secondObj) const
	{
		return (GetDomain() == secondObj.GetDomain() && GetUsernameHash() == secondObj.GetUsernameHash());
	}

private:
	void ParseAddress();
};