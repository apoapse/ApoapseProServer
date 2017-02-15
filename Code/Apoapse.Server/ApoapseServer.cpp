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

	m_usersManager = std::make_unique<UsersManager>(*this);

	auto cmd = CommandsManager::GetInstance().CreateCommand("CONVERSATION");
	cmd->SetInputRealFormat(Format::JSON);
	cmd->AppendCommandBodyData("{\"uuid\":\"81128ddee533436a9a302c8767bcd458\", \"correspondents\": [\"apoapse.space:9f86d081884c7d659a2feaa0c55ad015a3bf4f1b2b0b822cd15d6c15b0f00a08\",  \"apoapse.space:9f86d081884c7d6u9a2feaa0c55adb15a3bf4f1b2b0b822cd15d6c15b0f00a08\"] }");
	cmd->ParseRawCmdBody();
	LOG_DEBUG << "Is valid: " << cmd->IsValid();
	if (cmd->IsValid())
		cmd->ProcessFromNetwork(new LocalUser(GetUsersManager(), ApoapseAddress::UsernameHash("9f86d081884c7d659a2feaa0c55ad015a3bf4f1b2b0b822cd15d6c15b0f00a08"), 1), *(new ClientConnection(m_IOServiceForClients, *this)));



	

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