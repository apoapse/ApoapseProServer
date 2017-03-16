#include "stdafx.h"
#include "ApoapseServer.h"
#include "Common.h"

#include "TCPServer.h"
#include "UsersManager.h"
#include "CommandsManager.h"
#include "Command.h"
#include "ClientConnection.h"

#include "RemoteServersManager.h"
#include "RemoteServer.h"

ApoapseServer::ApoapseServer(UInt16 port, UInt16 serversPort, Database& db) : database(db)
{
	m_ServerForClients = std::make_unique<TCPServer>(m_IOServiceForClients, port/*, TCPServer::IP_v6*/);
	m_ServerForRemoteServers = std::make_unique<TCPServer>(m_IOServiceForRemoteServers, serversPort/*, TCPServer::IP_v6*/);
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
// 		
// 	std::vector<byte> data{ 'a', 'o', 'A'};
// 
// 	SQLQuery test1(database);
// 	test1 << INSERT_INTO << "test (blob_field)" << VALUES << "(" << data << ")";
// 	test1.Exec();
// 
// 	SQLQuery test2(database);
// 	test2 << SELECT << "blob_field" << FROM "test" << ORDER_BY << "id" << DESC << LIMIT << "1";
// 	auto res = test2.Exec();
// 	std::vector<byte> dataRes = res[0][0].GetByteArray();
// 	

	SQLQuery test1(database);
	test1 << SELECT << "timestamp" << FROM << "operations_log" << ORDER_BY << "id" << DESC;
	auto res = test1.Exec();
	if (res.RowCount() > 0) LOG << res[0][0].GetInt64();

	m_usersManager = std::make_unique<UsersManager>(*this);
	m_remoteServersManager = std::make_unique<RemoteServersManager>(m_IOServiceForRemoteServers);

	// Clients connections
	std::thread threadClients([this]
	{
		m_ServerForClients->StartAccept<ClientConnection>(std::ref(*this));
		m_IOServiceForClients.run();
	});
	threadClients.detach();

	// Remote servers connections
	std::thread threadServers([this]
	{
		m_ServerForRemoteServers->StartAccept<RemoteServer>(RemoteServer::Direction::REMOTE_CONNECTED_TO_CURRENT_SERVER, std::ref(*this));
		m_IOServiceForRemoteServers.run();
	});
	threadServers.detach();
}

UsersManager& ApoapseServer::GetUsersManager() const
{
	return *m_usersManager;
}

RemoteServersManager& ApoapseServer::GetRemoteServersManager() const
{
	return *m_remoteServersManager;
}
