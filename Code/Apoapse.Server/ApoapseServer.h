#pragma once

struct MyStruct
{
	MyStruct()
	{
		LOG << "Created";
	}

	~MyStruct()
	{
		LOG << "Deleted";
	}
};

class ApoapseServer
{
	static std::vector<string> m_requiredCommands;	//TODO

public:
	ApoapseServer(UInt16 port);
	~ApoapseServer();

	void Start();

	//void Send(const User& user);
	//void Send(const TCPConnection& connection);
private:
	void Test(std::unique_ptr<MyStruct> test)
	{

	}
};