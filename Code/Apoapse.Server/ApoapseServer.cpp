#include "stdafx.h"
#include "ApoapseServer.h"
#include "Common.h"

#include "NetMessage.h"

ApoapseServer::ApoapseServer()
{
	NetMessage::SetMaxAllowedSize();
}

ApoapseServer::~ApoapseServer()
{
}