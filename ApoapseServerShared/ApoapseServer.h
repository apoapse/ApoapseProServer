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
#include "TCPServer.h"
#include "TCPServerNoTLS.h"
#include "DataStructure.h"
#include "DatabaseSettings.h"
class UsersManager;
class UsergroupManager;

class ApoapseServer
{
	std::unique_ptr<TCPServer> m_mainServer;
	std::unique_ptr<TCPServer> m_filesServer;
	ssl::context m_tlsContext;

public:
	UsersManager* usersManager = nullptr;
	UsergroupManager* usergroupManager = nullptr;
	DatabaseSettings serverSettings;

	boost::asio::io_service fileServerIOService;
	boost::asio::io_service mainServerIOService;

	ApoapseServer();
 	virtual ~ApoapseServer();
	void SetupMainServer(UInt16 port);
	void SetupFilesServer(UInt16 port);
	void StartIOServices();

private:
};