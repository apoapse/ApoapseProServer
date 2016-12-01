#include "stdafx.h"
#include "Common.h"
#include "TestConnection.h"

#include "UnicodeUtils.h"
#include <codecvt>
#include <boost/property_tree/ptree.hpp>
#include <boost\property_tree\json_parser.hpp>

TestConnection::TestConnection(boost::asio::io_service& io_service) : TCPConnection(io_service)
{

}

TestConnection::~TestConnection()
{
	LOG_DEBUG_FUNCTION_NAME();
}

bool TestConnection::OnAcceptedByServer()
{
	LOG_DEBUG_FUNCTION_NAME();
	return true;
}

bool TestConnection::OnConnectedToServer(const boost::system::error_code& error)
{
	if (!error)
	{
		LOG << __FUNCTION__ << " connected to/from port " << GetEndpoint().port();

		string msg = "FROM SERVER!";
		//std::wstring wideMsg = Unicode::utf8_to_wstring(msg);

		return true;
	}
	else
	{
		LOG << LogSeverity::error << __FUNCTION__ << " " << error.message();
		return false;
	}
}

bool TestConnection::OnReceivedPacket(std::shared_ptr<NetMessage> netMessage)
{
	auto utf8_string = std::string(netMessage->GetRawData().begin(), netMessage->GetRawData().end());

	std::stringstream ss;
	ss << utf8_string;

	boost::property_tree::ptree propertyTree;
	boost::property_tree::read_json(ss, propertyTree);

	auto test = propertyTree.get<string>("test.testp1");

	//TO UTF-16
	// https://social.msdn.microsoft.com/Forums/vstudio/en-US/8f40dcd8-c67f-4eba-9134-a19b9178e481/vs-2015-rc-linker-stdcodecvt-error?forum=vcgeneral
	// https://connect.microsoft.com/VisualStudio/feedback/details/1403302/unresolved-external-when-using-codecvt-utf8
	// http://en.cppreference.com/w/cpp/locale/codecvt_utf8_utf16
	// http://stackoverflow.com/questions/7232710/convert-between-string-u16string-u32string
	/*std::wstring_convert<std::codecvt_utf8_utf16<char16_t>, char16_t> conv;
	//std::wstring_convert<std::codecvt<char16_t, char, std::mbstate_t>, char16_t> conv;

	std::u16string utf16_string = conv.from_bytes(utf8_string);

	*/
	Send(test);

	LOG << __FUNCTION__/* << "(length: " << utf16_string.length() << " " << Unicode::wstring_to_u8string(unicodeMsg)*/;
	//Send("From server: " + Unicode::wstring_to_u8string(unicodeMsg));
	return true;
}

bool TestConnection::OnReadError(const boost::system::error_code& error)
{
	LOG << LogSeverity::error << "TestConnection::OnReadError " << error.message();
	return false;
}

bool TestConnection::OnSentPacket(const std::shared_ptr<NetMessage> netMessage, size_t /*bytesTransferred*/)
{
	return true;
}