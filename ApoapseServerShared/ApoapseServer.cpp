#include "stdafx.h"
#include "ApoapseServer.h"
#include "Common.h"
#include "ServerConnection.h"
#include "UsersManager.h"
#include "SQLQuery.h"

ApoapseServer::~ApoapseServer()
{
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

	std::thread threadMainServer([this]
	{
		ssl::context tlsContext(ssl::context::sslv23);

// 		tlsContext.set_options(ssl::context::default_workarounds | ssl::context::no_sslv2/* | ssl::context::single_dh_use*/);
// 		tlsContext.use_certificate_chain_file("ca.pem");
// 		tlsContext.use_private_key_file("ca.pem", boost::asio::ssl::context::pem);

		tlsContext.set_options(boost::asio::ssl::context::default_workarounds | boost::asio::ssl::context::no_sslv2 | boost::asio::ssl::context::single_dh_use);
		//tlsContext.set_password_callback(boost::bind(&server; ::get_password, this));
		tlsContext.use_certificate_chain_file("user.crt");
		tlsContext.use_private_key_file("user.key", boost::asio::ssl::context::pem);
		tlsContext.use_tmp_dh_file("dh2048.pem");

		m_mainServer->StartAccept<ServerConnection>(this, std::ref(tlsContext));
		m_mainServerIOService.run();
	});
	threadMainServer.detach();
}
