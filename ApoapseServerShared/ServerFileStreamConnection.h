#pragma once
#include "FileStreamConnection.h"
class ApoapseServer;
class ServerConnection;

class ServerFileStreamConnection : public FileStreamConnection
{
	ServerConnection* m_mainConnection = nullptr;

public:
	ApoapseServer& server;
	
	ServerFileStreamConnection(boost::asio::io_service& ioService, ApoapseServer* server, ssl::context& context);

	ServerConnection* GetMainConnection() const;
	void SetMainConnection(ServerConnection* serverConnection);
	
protected:
	void OnFileDownloadCompleted() override;
	void ErrorDisconnectAll() override;
	void Authenticate(const Username& username, const hash_SHA256& authCode) override;
	std::string GetDownloadFilePath(UInt64 fileSize) override;
	void OnFileSentSuccessfully() override;
	void OnConnectedToServer() override;
};