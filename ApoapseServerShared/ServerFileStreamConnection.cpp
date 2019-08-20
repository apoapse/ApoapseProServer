#include "stdafx.h"
#include "Common.h"
#include "ServerFileStreamConnection.h"
#include <boost/asio/ssl/context.hpp>
#include "ApoapseServer.h"
#include <filesystem>
#include "UsersManager.h"

ServerFileStreamConnection::ServerFileStreamConnection(boost::asio::io_service& ioService, ApoapseServer* server, boost::asio::ssl::context& context)
	: FileStreamConnection(ioService, context)
	, server(*server)
{
}

ServerConnection* ServerFileStreamConnection::GetMainConnection() const
{
	return m_mainConnection;
}

void ServerFileStreamConnection::SetMainConnection(ServerConnection* serverConnection)
{
	m_mainConnection = serverConnection;
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
	global->mainThread->PushTask([this, &username, &authCode]()
	{
		auto user = server.usersManager->GetUserByUsername(username);
		user.lock()->AuthenticateFileStream(authCode, this);
	});

	//std::uintmax_t size = std::filesystem::file_size("_test_img.jpg");
	//SendFile("_test_img.jpg", size);
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
