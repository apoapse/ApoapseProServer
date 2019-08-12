#pragma once
#include "FileStreamConnection.h"
class ApoapseServer;

class ServerFileStreamConnection : public FileStreamConnection
{
public:
	ServerFileStreamConnection(boost::asio::io_service& ioService, ApoapseServer* server, ssl::context& context);
	
protected:
	void OnFileDownloadCompleted() override;
	void ErrorDisconnectAll() override;
	void Authenticate(const Username& username, const hash_SHA256& authCode) override;
	std::string GetDownloadFilePath(UInt64 fileSize) override;
	void OnFileSentSuccessfully() override;
	void OnConnectedToServer() override;
};