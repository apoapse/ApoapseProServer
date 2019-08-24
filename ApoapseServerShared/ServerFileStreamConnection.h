#pragma once
#include "FileStreamConnection.h"
#include <deque>
#include "Uuid.h"
class ApoapseServer;
class ServerConnection;
/*
struct Attachment
{
	Uuid uuid;
	Uuid parentThread;
	std::string fileName;
	size_t fileSize = 0;

	Attachment() = default;
	Attachment(DataStructure& data);
};
*/

class ServerFileStreamConnection : public FileStreamConnection
{
	ServerConnection* m_mainConnection = nullptr;

public:
	ApoapseServer& server;
	
	ServerFileStreamConnection(boost::asio::io_service& ioService, ApoapseServer* server, ssl::context& context);

	ServerConnection* GetMainConnection() const;
	void SetMainConnection(ServerConnection* serverConnection);
	
protected:
	void ErrorDisconnectAll() override;
	void Authenticate(const Username& username, const hash_SHA256& authCode) override;
	void OnSocketConnected() override;
	void OnFileDownloadCompleted(const AttachmentFile& file) override;
	void OnFileSentSuccessfully(const AttachmentFile& file) override;
};