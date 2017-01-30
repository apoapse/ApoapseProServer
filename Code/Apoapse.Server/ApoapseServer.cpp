#include "stdafx.h"
#include "ApoapseServer.h"
#include "Common.h"

#include "TCPServer.h"
#include "UsersManager.h"
#include "CommandsManager.h"
#include "Command.h"
#include "ClientConnection.h"
#include "LocalUser.h"//TEMP
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
// 	string name = "MyName5";
// 	int age = 23;
// 	SQLQuery test1(database);
// 	test1 << INSERT_INTO << "demo" << VALUES << "(" << name << "," << age << ")";
// 	//test1.Exec();
// 
// 	SQLQuery test(database);
// 	test << SELECT << "name" << FROM << "demo";
// 	auto testd = test.ExecAsync();
// 	auto result = testd.get();
// 
// 	LOG << result[0][ROW_HELPER("name", 0)].GetText() << LogSeverity::debug;
// 
// 	if (result)
// 	{
// 		for (size_t i = 0; i < result.RowCount(); i++)
// 		{
// 			for (auto& column : result[i])
// 			{
// 				LOG << column.GetText();
// 			}
// 		}
// 	}
// 	else
// 		LOG "sql res error";

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