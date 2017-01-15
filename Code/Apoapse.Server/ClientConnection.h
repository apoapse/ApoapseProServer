#pragma once
#include "GenericConnection.h"
#include <boost/optional.hpp>

class LocalUser;

class ClientConnection : public GenericConnection
{
	boost::optional<std::shared_ptr<LocalUser>> m_associatedUser;

public:
	ClientConnection(boost::asio::io_service& io_service, ApoapseServer& apoapseServer);
	virtual ~ClientConnection();

	bool HasAssociatedUser() const;
	std::shared_ptr<LocalUser> GetAssociatedUser() const;
	void SetAssociatedUser(std::shared_ptr<LocalUser> user);

	bool CheckCommandNetworkInputCompatibility(Command& command) override;
	void ProcessCommandFromNetwork(Command& command) override;
private:
};