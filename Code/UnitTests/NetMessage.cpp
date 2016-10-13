#include "stdafx.h"
#include "CppUnitTest.h"
#include "Apoapse.Network\NetMessage.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

TEST_CLASS(NetMessageTests)
{
public:
	/*TEST_METHOD(CreateNetMessageFromRawHeader)
	{
		auto dataHeader = boost::array<char, SOCKET_READ_BUFFER_SIZE> { 'I', '1', '6' };
		try
		{
			NetMessage netMessage = NetMessage::FromNetRawHeader(dataHeader, dataHeader.size());

			Assert::IsTrue(netMessage.GetMessageType() == NetMessageType::info);
		}
		catch (const std::exception&)
		{
			Assert::Fail();
		}
	}

	TEST_METHOD(CreateNetMessageFromString)
	{
		NetMessage netMessage(NetMessageType::command, 10);
		netMessage.PushData("TEST");
		netMessage.PushData("TEST2é");

		auto data = netMessage.GetData();
		string str(data.begin(), data.end());

		Assert::IsTrue(str == "TESTTEST2é");
	}

	TEST_METHOD(PushDataCharArray)
	{
		auto dataSet1 = boost::array<char, SOCKET_READ_BUFFER_SIZE> { 'z', 'U' };
		auto dataSet2 = boost::array<char, SOCKET_READ_BUFFER_SIZE> { 'X', ' ', '_' };

		NetMessage netMessage(NetMessageType::command, 5);

		netMessage.PushData(dataSet1, 2);
		netMessage.PushData(dataSet2, 3);

		auto data = netMessage.GetData();
		string str(data.begin(), data.end());

		Assert::IsTrue(str == "zUX _");
	}*/
};