#pragma once

struct INetworkSender
{
	virtual void Send(std::shared_ptr<std::vector<byte>> bytesPtr) = 0;
	virtual void Send(std::unique_ptr<std::string> strPtr) = 0;
};