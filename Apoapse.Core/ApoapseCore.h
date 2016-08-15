#pragma once
#include "ILoadCoreDLLAPI.h"
#include <boost/config.hpp>

class ApoapseCore : public ILoadCoreDLLAPI
{
public:
	ApoapseCore()
	{
	}
	~ApoapseCore();

	void Start(Global*);
private:

};

extern "C" BOOST_SYMBOL_EXPORT ApoapseCore core_dll;
ApoapseCore core_dll;