// ----------------------------------------------------------------------------
// Copyright (C) 2020 Apoapse
// Copyright (C) 2020 Guillaume Puyal
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at http://www.apache.org/licenses/LICENSE-2.0
//
// For more information visit https://github.com/apoapse/
// And https://apoapse.space/
// ----------------------------------------------------------------------------

#pragma once
#include "TypeDefs.hpp"
#include "CommandsManagerV2.h"
class User;

class ServerCmdManager : public CommandsManagerV2
{
public:
	ServerCmdManager();

	// Inherited via CommandsManagerV2
	bool OnSendCommandPre(CommandV2& cmd) override;
	bool OnReceivedCommandPre(CommandV2& cmd, GenericConnection& netConnection) override;
	void OnReceivedCommand(CommandV2& cmd, GenericConnection& netConnection) override;
	void OnReceivedCommandPost(CommandV2& cmd, GenericConnection& netConnection) override;
	void Propagate(CommandV2& cmd, GenericConnection& localConnection) override;
	void PropagateToUser(CommandV2& cmd, User& user);

private:
	static std::string GenerateAttachmentPath(const Uuid& fileUuid);
};
