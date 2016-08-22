#include "stdafx.h"
#include "ApoapseNetwork.h"

Global* global = nullptr;

void ApoapseNetwork::Start(Global* outsideGlobalPtr)
{
	ASSERT(global == nullptr);

	global = outsideGlobalPtr;
	ASSERT(global);

	Log("ApoapseNetwork started");
}

ApoapseNetwork::~ApoapseNetwork()
{

}