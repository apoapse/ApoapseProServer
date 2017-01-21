#include "stdafx.h"
#include "ApoapseServer.h"
#include "Common.h"

#include "CommandsManager.h"
#include "Command.h"
#include "ClientConnection.h"

ApoapseServer::ApoapseServer(UInt16 port, Database& db) : database(db)
{
	m_usersManager = std::make_unique<UsersManager>();
	m_ServerForClients = std::make_unique<TCPServer>(m_IOServiceForClients, port/*, TCPServer::IP_v6*/);
}

ApoapseServer::~ApoapseServer()
{
}

void ApoapseServer::Start()
{
	SQLQuery test(database);
	test << SELECT << "name" << FROM << "demo";
	auto testd = test.ExecAsync();
	auto result = testd.get();
	if (result)
	{
		for (size_t i = 0; i < result.RowCount(); i++)
		{
			for (auto& column : result[i])
			{
				LOG << column.GetTextStr();
			}
		}
	}
	else
		LOG "sql res error";

	std::thread thread([this]
	{
		m_ServerForClients->StartAccept<ClientConnection>(std::ref(*this));
		m_IOServiceForClients.run();
	});
	thread.detach();
}

UsersManager& ApoapseServer::GetUsersManager() const
{
	return *m_usersManager;
}