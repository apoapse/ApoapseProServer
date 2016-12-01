#pragma once

class ApoapseServer
{
	static std::vector<string> m_requiredCommands;	//TODO

public:
	ApoapseServer(UInt16 port);
	~ApoapseServer();

	//void Send(const User& user);
	//void Send(const TCPConnection& connection);
private:

};