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

ApoapseServer::ApoapseServer() : m_tlsContext(ssl::context(ssl::context::sslv23))
{
	m_tlsContext.set_options(boost::asio::ssl::context::default_workarounds | boost::asio::ssl::context::no_sslv2 | boost::asio::ssl::context::single_dh_use);
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
	
	m_mainServer = std::make_unique<TCPServer>(m_mainServerIOService, port, TCPServer::Protocol::ip_v6);
	m_mainServer->StartAccept<ServerConnection>(this, std::ref(m_tlsContext));
}

void ApoapseServer::SetupFilesServer(UInt16 port)
{
	m_filesServer = std::make_unique<TCPServer>(m_fileServerIOService, port, TCPServer::Protocol::ip_v6);
	m_filesServer->StartAccept<ServerFileStreamConnection>(this, std::ref(m_tlsContext));
}

void ApoapseServer::StartIOServices()
{
	boost::thread_group threads;

	// Main server
	threads.create_thread([this]
	{
		ThreadUtils::NameThread("Main server");
		m_mainServerIOService.run();
	});

	// File stream server (two thread used)
	threads.create_thread([this]
	{
		ThreadUtils::NameThread("File Stream server #1");
		m_fileServerIOService.run();
	});
	threads.create_thread([this]
	{
		ThreadUtils::NameThread("File Stream server #2");
		m_fileServerIOService.run();
	});
	
	threads.join_all();
}
