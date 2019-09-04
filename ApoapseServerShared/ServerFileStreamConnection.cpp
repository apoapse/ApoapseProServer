#include "stdafx.h"
#include "Common.h"
#include "ServerFileStreamConnection.h"
#include <boost/asio/ssl/context.hpp>
#include "ApoapseServer.h"
#include <filesystem>
#include "UsersManager.h"
#include "ServerConnection.h"
/*
Attachment::Attachment(DataStructure& data)
{
	uuid = data.GetField("uuid").GetValue<Uuid>();
	parentThread = data.GetField("parent_thread").GetValue<Uuid>();
	fileName = data.GetField("name").GetValue<std::string>();
	fileSize = data.GetField("file_size").GetValue<Int64>();
}*/

ServerFileStreamConnection::ServerFileStreamConnection(boost::asio::io_service& ioService, ApoapseServer* server/*, boost::asio::ssl::context& context*/)
	: FileStreamConnection(ioService/*, context*/)
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

void ServerFileStreamConnection::ErrorDisconnectAll()
{
	LOG_DEBUG << "ErrorDisconnectAll";
	
	GetMainConnection()->Close();
	Close();
}

void ServerFileStreamConnection::Authenticate(const Username& username, const hash_SHA256& authCode)
{
	global->mainThread->PushTask([this, &username, &authCode]()
	{
		auto user = server.usersManager->GetUserByUsername(username);
		user.lock()->AuthenticateFileStream(authCode, this);
	});
}

void ServerFileStreamConnection::OnSocketConnected()
{
}

void ServerFileStreamConnection::OnFileDownloadCompleted(const AttachmentFile& file)
{
	LOG_DEBUG << "OnFileDownloadCompleted " << file.fileName;

	global->mainThread->PushTask([file]()
	{
		auto dat = global->apoapseData->ReadItemFromDatabase("attachment", "uuid", file.uuid);
		dat.GetField("is_downloaded").SetValue(true);
		dat.SaveToDatabase();
	});
}

void ServerFileStreamConnection::OnFileSentSuccessfully(const AttachmentFile& file)
{
	LOG << "File sent successfully " << file.fileName;
}
