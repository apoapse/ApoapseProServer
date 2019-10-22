#include "stdafx.h"
#include "ApoapseServer.h"
#include "Common.h"
#include "ServerConnection.h"
#include "UsersManager.h"
#include "UsergroupManager.h"
#include "ServerFileStreamConnection.h"
#include <boost/thread.hpp>
#include "ServerSettings.h"
#include "ThreadUtils.h"
#include "TCPServerNoTLS.h"
#include "SQLUtils.hpp"
#include "Random.hpp"

ApoapseServer::ApoapseServer() : m_tlsContext(ssl::context(ssl::context::sslv23))
{
	m_tlsContext.set_options(ssl::context::default_workarounds | ssl::context::no_sslv2 | ssl::context::no_tlsv1_1 | ssl::context::no_tlsv1 | ssl::context::single_dh_use/* | ssl::context::no_compression*/);
	m_tlsContext.set_default_verify_paths();
	m_tlsContext.use_certificate_chain_file("server.crt");
	m_tlsContext.use_private_key_file("server.key", boost::asio::ssl::context::pem);
	m_tlsContext.use_tmp_dh_file("dh2048.pem");
}

ApoapseServer::~ApoapseServer()
{
	delete usersManager;
	delete usergroupManager;
}

void ApoapseServer::SetupMainServer(UInt16 port)
{
	{
		if (SQLUtils::CountRows("server_settings") == 0)
		{
			SQLQuery query(*global->database);
			query << INSERT_INTO << "server_settings" << VALUES  << "(NULL, NULL, NULL)";
			query.Exec();

			LOG << "server_settings table empty, row created";
		}

		serverSettings = DatabaseSettings("server_setting");
	}

	constexpr int serverPrefixLength = 5;
	if (serverSettings.GetValueOr<std::string>("server_prefix", ""s).length() != serverPrefixLength)
	{
		const std::string serverPrefix = Cryptography::GenerateRandomCharacters(serverPrefixLength, serverPrefixLength, false);
		serverSettings.SetValue("server_prefix", serverPrefix);
		LOG << "Server random prefix set to " << serverPrefix;
	}
	
	usersManager = new UsersManager;
	usergroupManager = new UsergroupManager;

	m_mainServer = std::make_unique<TCPServer>(mainServerIOService, port, TCPServer::Protocol::ip_v6);
	m_mainServer->StartAccept<ServerConnection>(this, std::ref(m_tlsContext));
}

void ApoapseServer::SetupFilesServer(UInt16 port)
{
	m_filesServer = std::make_unique<TCPServer>(fileServerIOService, port, TCPServer::Protocol::ip_v6);
	m_filesServer->StartAccept<ServerFileStreamConnection>(this, std::ref(m_tlsContext));
}

void ApoapseServer::StartIOServices()
{
	boost::thread_group threads;

	const int cpuThreadCount = std::thread::hardware_concurrency();
	const int mainConnectionsThreadCount = std::max(std::ceil(((float)cpuThreadCount - 1.0f) / 2.0f), 1.0f);
	const int fileStreamsThreadCount = std::max(mainConnectionsThreadCount - 1, 1);
	
	// Main connections
	for (int i = 0; i < mainConnectionsThreadCount; ++i)
	{
		threads.create_thread([this, i]
		{
			{
				std::stringstream threadName;
				threadName << "Main connection " << "#" << i + 1;
				ThreadUtils::NameThread(threadName.str());
			}
			
			mainServerIOService.run();
		});
	}

	// File streams
	for (int i = 0; i < fileStreamsThreadCount; ++i)
	{
		threads.create_thread([this, i]
		{
			{
				std::stringstream threadName;
				threadName << "File Stream server " << "#" << i + 1;
				ThreadUtils::NameThread(threadName.str());
			}

			fileServerIOService.run();
		});
	}
	
	// Main thread
	ThreadUtils::NameThread("Apoapse Main Thread");
	global->mainThread->Run();
//	threads.join_all();
}
