#include "stdafx.h"
#include "CmdServerInfo.h"
#include "Common.h"
#include "CommandsManager.h"
#include "User.h"

void CmdServerInfo::SendSetupState(INetworkSender& destination)
{
	MessagePackSerializer ser;
	ser.UnorderedAppend<std::string>("status", "setup_state");

	Send(ser, destination);
}

void CmdServerInfo::SendWellcome(INetworkSender& destination, const User& user)
{
	const auto[encryptedPrivateKey, iv] = user.GetEncryptedPrivateKey();

	MessagePackSerializer ser;
	ser.UnorderedAppend<std::string>("status", "authenticated");
	ser.UnorderedAppend("public_key", user.GetPublicKey());
	ser.UnorderedAppend("private_key_encrypted", encryptedPrivateKey);
	ser.UnorderedAppend("private_key_iv", iv);

	Send(ser, destination);
}

CommandInfo& CmdServerInfo::GetInfo() const
{
	static auto info = CommandInfo();
	info.command = CommandId::server_info;
	info.clientOnly = true;
	info.onlyNonAuthenticated = true;

	return info;
}

APOAPSE_COMMAND_REGISTER(CmdServerInfo, CommandId::server_info);