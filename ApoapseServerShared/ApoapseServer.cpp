#include "stdafx.h"
#include "ApoapseServer.h"
#include "Common.h"
#include "ServerConnection.h"
#include "UsersManager.h"
#include "UsergroupsManager.h"
#include "SQLQuery.h"

ApoapseServer::~ApoapseServer()
{
	delete usergroupsManager;
	delete usersManager;
}

void ApoapseServer::StartMainServer(UInt16 port)
{
	m_mainServer = std::make_unique<TCPServer>(m_mainServerIOService, port, TCPServer::Protocol::ip_v6);

	// TEMP
	/*{
		SQLQuery query(*global->database);
		query << DELETE_FROM << "usergroups";
		query.Exec();
	}

	{
		SQLQuery query(*global->database);
		query << DELETE_FROM << "usergroups_blockchain";
		query.Exec();
	}

	{
		SQLQuery query(*global->database);
		query << DELETE_FROM << "users";
		query.Exec();
	}*/
	// END TEMP

	/*
	// List registered users
	{
		SQLQuery query(*global->database);
		query << SELECT << "username_hash" << FROM << "users";
		auto res = query.Exec();

		for (const auto& item : res)
		{
			LOG_DEBUG << "username_hash: " << item[0].GetByteArray();
		}
	}*/

	usersManager = new UsersManager;
	usergroupsManager = new UsergroupsManager(*usersManager);
	usergroupsManager->Init();

	std::thread threadMainServer([this]
	{
		m_mainServer->StartAccept<ServerConnection>(this);
		m_mainServerIOService.run();
	});
	threadMainServer.detach();
}
