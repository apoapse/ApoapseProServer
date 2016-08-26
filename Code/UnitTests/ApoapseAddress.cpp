#include "stdafx.h"
#include "CppUnitTest.h"
#include "Apoapse.Network\ApoapseAddress.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

TEST_CLASS(ApoapseAddressTests)
{
public:
	TEST_METHOD(InitiateValidAddress)
	{
		ApoapseAddress* address = ApoapseAddress::Create("apoapse.space:myusername");

		Assert::IsTrue(address->GetDomain() == "apoapse.space" && address->GetUsername() == "myusername");
	}

	TEST_METHOD(GetFullAddressFromApoapseAddress)
	{
		ApoapseAddress* address = ApoapseAddress::Create("apoapse.space:myusername");

		Assert::IsTrue(address->GetFullAddress() == "apoapse.space:myusername");
	}

	TEST_METHOD(CompareSameAddresses)
	{
		ApoapseAddress* address1 = ApoapseAddress::Create("apoapse.space:myusername");
		ApoapseAddress* address2 = ApoapseAddress::Create("apoapse.space:myusername");

		Assert::IsTrue((*address1) == (*address2));
	}

	TEST_METHOD(CompareDifferentAddresses)
	{
		ApoapseAddress* address1 = ApoapseAddress::Create("apoapse.space:myusername");
		ApoapseAddress* address2 = ApoapseAddress::Create("apoapse.space:myusername2");

		Assert::IsFalse((*address1) == (*address2));
	}

	TEST_METHOD(ValidateValidAddress)
	{
		ApoapseAddress* address = ApoapseAddress::Create("apoapse.space:myusername");

		Assert::IsTrue(address->IsValid());
	}

	TEST_METHOD(ValidateInvalidAddress_syntaxt)
	{
		ApoapseAddress* address = ApoapseAddress::Create("apoapse.spacemyusername");

		Assert::IsFalse(address->IsValid());
	}

	TEST_METHOD(ValidateInvalidAddress_domain)
	{
		ApoapseAddress* address = ApoapseAddress::Create("apoapsespace:myusename");

		Assert::IsFalse(address->IsValid());
	}

	TEST_METHOD(ValidateInvalidAddress_username)
	{
		ApoapseAddress* address = ApoapseAddress::Create("apoapse.space:myuse<strong>name</strong>");

		Assert::IsFalse(address->IsValid());
	}
};