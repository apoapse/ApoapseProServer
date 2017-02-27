#pragma once
class TCPConnection;

struct INetworkSender
{
	//************************************
	// Method:    INetworkSender::Send - Send a byte array to a connection or a list of connections (in the case of an user for instance)
	// Access:    virtual public 
	// Returns:   void
	// Parameter: std::shared_ptr<std::vector<byte>> bytesPtr - Byte array shared pointer. A smart pointer is used because TCPConnection need ownership and persistance of the data.
	// Parameter: GenericConnection * excludedConnection - In case the implementation of this function send to multiple connections, this argument can be used to exclude one particular connection where the data will not be sent.
	//************************************
	virtual void Send(std::shared_ptr<std::vector<byte>> bytesPtr, TCPConnection* excludedConnection = nullptr) = 0;

	//************************************
	// Method:    INetworkSender::Send - Send a string to a connection or a list of connections (in the case of an user for instance)
	// Access:    virtual public 
	// Returns:   void
	// Parameter: std::unique_ptr<std::string> strPtr - String unique pointer. A smart pointer is used because TCPConnection need ownership and persistance of the data.
	// Parameter: GenericConnection * excludedConnection - In case the implementation of this function send to multiple connections, this argument can be used to exclude one particular connection where the data will not be sent.
	//************************************
	virtual void Send(std::unique_ptr<std::string> strPtr, TCPConnection* excludedConnection = nullptr) = 0;
};