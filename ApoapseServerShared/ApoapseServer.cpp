#include "stdafx.h"
#include "ApoapseServer.h"
#include "Common.h"
#include "ServerConnection.h"
#include "UsersManager.h"
#include "UsergroupManager.h"
#include "ServerFileStreamConnection.h"
#include <boost/thread.hpp>
#include "ServerSettings.h"

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
	m_mainServer = std::make_unique<TCPServer>(m_mainServerIOService, port, TCPServer::Protocol::ip_v6);

	usersManager = new UsersManager;
	usergroupManager = new UsergroupManager;

	m_mainServer->StartAccept<ServerConnection>(this, std::ref(m_tlsContext));
}

void ApoapseServer::SetupFilesServer(UInt16 port)
{
	m_filesServer = std::make_unique<TCPServer>(m_fileServerIOService, port, TCPServer::Protocol::ip_v6);

	m_filesServer->StartAccept<ServerFileStreamConnection>(this, std::ref(m_tlsContext));
	m_mainServerIOService.run_one();
}

void ApoapseServer::StartIOServices()
{
	boost::thread_group threads;

	threads.create_thread([this]
	{
		m_mainServerIOService.run();
	});

	threads.create_thread([this]
	{
		m_mainServerIOService.run();
	});
	
	threads.join_all();
}
