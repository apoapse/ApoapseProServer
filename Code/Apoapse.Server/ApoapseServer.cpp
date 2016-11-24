#include "stdafx.h"
#include "ApoapseServer.h"
#include "Common.h"

#include "NetMessage.h"
#include "TCPServer.h"

ApoapseServer::ApoapseServer(UInt16 port)
{
	NetMessage::SetMaxAllowedSize();
}

ApoapseServer::~ApoapseServer()
{
}