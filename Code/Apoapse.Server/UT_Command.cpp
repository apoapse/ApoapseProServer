#include "stdafx.h"

#ifdef UNIT_TESTS

#include "Common.h"
#include "Apoapse.UnitTest/TestsManager.h"
#include "Command.h"
#include "CommandsManager.h"
#include "INetworkSender.h"

struct TestSend : INetworkSender
{
	string sentData;

	void Send(std::unique_ptr<std::string> strPtr, TCPConnection* /*excludedConnection*/ = nullptr) override
	{
		sentData = *strPtr;
	}

	void Send(std::shared_ptr<std::vector<byte>> bytesPtr, TCPConnection* /*excludedConnection*/ = nullptr) override
	{
		ASSERT(false);
	}
};

UNIT_TEST("CommandSystem:Config")
{
	auto command = CommandsManager::GetInstance().CreateCommand("UNIT_TEST");

	UnitTest::Assert(command->GetConfig().name == "UNIT_TEST" && command->GetConfig().expectedFormat == Format::INLINE);

} UNIT_TEST_END

UNIT_TEST("CommandSystem:FromRawNetworkInput:INLINE")
{
	auto command = CommandsManager::GetInstance().CreateCommand("UNIT_TEST");
	command->SetInputRealFormat(Format::INLINE);
	command->AppendCommandBodyData("9 teststr");
	command->ParseRawCmdBody();

	UnitTest::Assert(command->ReadFieldValue<string>("test_string").get() == "teststr" && command->ReadFieldValue<UInt16>("test_uint16").get() == 9);
} UNIT_TEST_END

UNIT_TEST("CommandSystem:FromRawNetworkInput:JSON")
{
	auto command = CommandsManager::GetInstance().CreateCommand("UNIT_TEST");
	command->SetInputRealFormat(Format::JSON);
	command->AppendCommandBodyData("{\"test_uint16\": 9");
	command->AppendCommandBodyData(", \"test_string\": \"teststr\"}\n\n");
	command->ParseRawCmdBody();

	UnitTest::Assert(command->ReadFieldValue<string>("test_string").get() == "teststr" && command->ReadFieldValue<UInt16>("test_uint16").get() == 9);
} UNIT_TEST_END

UNIT_TEST("CommandSystem:Validate:WrongInputType")
{
	auto command = CommandsManager::GetInstance().CreateCommand("UNIT_TEST");
	command->SetInputRealFormat(Format::INLINE);
	command->AppendCommandBodyData("70000 teststr");
	command->ParseRawCmdBody();

	UnitTest::Assert(!command->IsValid());
} UNIT_TEST_END

UNIT_TEST("CommandSystem:Validate:MissingMandatoryValue")
{
	auto command = CommandsManager::GetInstance().CreateCommand("UNIT_TEST");
	command->SetInputRealFormat(Format::INLINE);
	command->AppendCommandBodyData("10");
	command->ParseRawCmdBody();

	UnitTest::Assert(!command->IsValid());
} UNIT_TEST_END

UNIT_TEST("CommandSystem:Send:ExpectedFormat")
{
	auto command = CommandsManager::GetInstance().CreateCommand("UNIT_TEST");
	command->SetInputRealFormat(Format::JSON);
	command->AppendCommandBodyData("{\"test_uint16\": 9, \"test_string\": \"teststr\"}");
	command->ParseRawCmdBody();

	TestSend sender;
	command->Send(sender);

	UnitTest::Assert(sender.sentData == "UNIT_TEST\n9 teststr\n", sender.sentData.c_str());
} UNIT_TEST_END

UNIT_TEST("CommandSystem:Send:ForceJson")
{
	auto command = CommandsManager::GetInstance().CreateCommand("UNIT_TEST");
	command->SetInputRealFormat(Format::INLINE);
	command->AppendCommandBodyData("9 teststr");
	command->ParseRawCmdBody();

	TestSend sender;
	command->Send(sender, Format::JSON);

	UnitTest::Assert(sender.sentData == "UNIT_TEST\n{\"test_uint16\":\"9\",\"test_string\":\"teststr\"}\n\n", sender.sentData.c_str());
} UNIT_TEST_END

#endif	// UNIT_TESTS