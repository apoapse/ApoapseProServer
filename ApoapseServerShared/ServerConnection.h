// ----------------------------------------------------------------------------
// Copyright (C) 2020 Apoapse
// Copyright (C) 2020 Guillaume Puyal
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at http://www.apache.org/licenses/LICENSE-2.0
//
// For more information visit https://github.com/apoapse/
// And https://apoapse.space/
// ----------------------------------------------------------------------------

#pragma once
#include "GenericConnection.h"
#include <optional>
#include "User.h"
class ApoapseServer;
class ServerFileStreamConnection;

class ServerConnection : public GenericConnection
{
	std::optional<std::shared_ptr<User>> m_relatedUser;
	ServerFileStreamConnection* m_fileStream = nullptr;

public:
	ApoapseServer& server;

	ServerConnection(boost::asio::io_service& ioService, ApoapseServer* server, ssl::context& context);
	virtual ~ServerConnection() override;

	virtual void Close() override;

	bool IsAuthenticated() const override;
	User& Authenticate(const Username& username);
	User* GetRelatedUser() const;
	std::optional<Username> GetConnectedUser() const override;
	void SetRelatedFileStream(ServerFileStreamConnection* fileStream);
	ServerFileStreamConnection* GetFileStream() const;
	
private:
	bool OnConnectedToServer() override;
	void OnReceivedCommand(CommandV2& cmd) override;
};