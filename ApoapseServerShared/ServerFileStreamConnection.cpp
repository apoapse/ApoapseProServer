#include "stdafx.h"
#include "Common.h"
#include "ServerFileStreamConnection.h"
#include <boost/asio/ssl/context.hpp>
#include "ApoapseServer.h"
#include <filesystem>

ServerFileStreamConnection::ServerFileStreamConnection(boost::asio::io_service& ioService, ApoapseServer* server, boost::asio::ssl::context& context)
	: FileStreamConnection(ioService, context)
	//, server(*server)
{
}

void ServerFileStreamConnection::OnFileDownloadCompleted()
{
	LOG_DEBUG << "Download completed";
}

void ServerFileStreamConnection::ErrorDisconnectAll()
{
	LOG_DEBUG << "ErrorDisconnectAll";
	
}

void ServerFileStreamConnection::Authenticate(const Username& username, const hash_SHA256& authCode)
{
	LOG_DEBUG << "Authenticate";

	//std::uintmax_t size = std::filesystem::file_size("_test_img.jpg");
	//SendFile("_test_img.jpg", size);
	
	std::uintmax_t size = std::filesystem::file_size("_testfile.mkv");
	SendFile("_testfile.mkv", size);
}

std::string ServerFileStreamConnection::GetDownloadFilePath(UInt64 fileSize)
{
	LOG_DEBUG << "GetDownloadFilePath";
	return "_testfile.mkv";
}

void ServerFileStreamConnection::OnFileSentSuccessfully()
{
	LOG_DEBUG << "OnFileSentSuccessfully";
	
}

void ServerFileStreamConnection::OnConnectedToServer()
{
	LOG_DEBUG << "OnConnectedToServer";
}
