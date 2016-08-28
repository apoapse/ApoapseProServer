#pragma once
#include <iostream>

struct Parrent
{
	virtual void SayHi() = 0;
	void SayHAHAHA()
	{
		std::cout << "HAHAHA" << std::endl;
	}
};

class MyClass : public Parrent
{
public:
	MyClass()
	{
		std::cout << ", LogSeverity::warning" << std::endl;
	}

	void SayHi()
	{
		std::cout << "HI HI HI!" << std::endl;
	}
};

template<class T_CONNECTION> class Test
{
public:
	Test()
	{
		Parrent* connection = new T_CONNECTION();
		connection->SayHi();
		connection->SayHAHAHA();
	}
};