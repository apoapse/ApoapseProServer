#pragma once

struct INetworkSender
{
	virtual void Send(const std::vector<byte>& bytes) = 0;
	virtual void Send(const std::string& str) = 0;
};