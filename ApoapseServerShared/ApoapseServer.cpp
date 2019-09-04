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

ApoapseServer::ApoapseServer() : m_tlsContext(ssl::context(ssl::context::sslv23))
{
	m_tlsContext.set_options(boost::asio::ssl::context::default_workarounds | boost::asio::ssl::context::no_sslv2 | boost::asio::ssl::context::single_dh_use);
	m_tlsContext.set_default_verify_paths();
	m_tlsContext.use_certificate_chain_file("user.crt");
	m_tlsContext.use_private_key_file("user.key", boost::asio::ssl::context::pem);
	m_tlsContext.use_tmp_dh_file("dh2048.pem");
}

ApoapseServer::~ApoapseServer()
{
	delete usersManager;
	delete usergroupManager;
}

void ApoapseServer::SetupMainServer(UInt16 port)
{
	usersManager = new UsersManager;
	usergroupManager = new UsergroupManager;

	global->mainConnectionIOService = std::make_unique<boost::asio::io_service>();
	m_mainServer = std::make_unique<TCPServer>(*global->mainConnectionIOService, port, TCPServer::Protocol::ip_v6);
	m_mainServer->StartAccept<ServerConnection>(this, std::ref(m_tlsContext));
}

void ApoapseServer::SetupFilesServer(UInt16 port)
{
	m_filesServer = std::make_unique<TCPServerNoTLS>(fileServerIOService, port, TCPServerNoTLS::Protocol::ip_v6);
	m_filesServer->StartAccept<ServerFileStreamConnection>(this/*, std::ref(m_tlsContext)*/);
}

void ApoapseServer::StartIOServices()
{
	boost::thread_group threads;

	const int cpuThreadCount = std::thread::hardware_concurrency();
	//const int mainConnectionsThreadCount = std::max(std::ceil(((float)cpuThreadCount - 1.0f) / 2.0f), 1.0f);
	const int mainConnectionsThreadCount = 1;
	//const int fileStreamsThreadCount = std::max(mainConnectionsThreadCount - 1, 1);
	const int fileStreamsThreadCount = 1;
	
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
			
			global->mainConnectionIOService->run();
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
