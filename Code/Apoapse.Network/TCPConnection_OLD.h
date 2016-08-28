#pragma once
#include <boost\asio.hpp>
#include <boost\enable_shared_from_this.hpp>

namespace TCPConnection_OLD
{

	class TCPConnection : public boost::enable_shared_from_this<TCPConnection>
	{
		using boostTCP = boost::asio::ip::tcp;

	public:
		TCPConnection(boost::asio::io_service& io_service);
		~TCPConnection();

		boostTCP::socket& GetSocket()
		{
			return m_socket;
		}

		typedef boost::shared_ptr<TCPConnection> pointer;
		static pointer Create(boost::asio::io_service& io_service)
		{
			return pointer(new TCPConnection(io_service));
		}

		void Connect(const std::string& adress, const unsigned short port);
		void Close();
		void OnAccepted();
		void Send(const boost::asio::const_buffer& inputBuffer);
		boost::asio::ip::tcp::endpoint GetEndpoint() const;
		bool IsConnected() const;

	private:
		void HandleConnectAsync(const boost::system::error_code&);
		void HandleWriteAsync(const boost::system::error_code&, size_t bytesTransferred);
		void ListenIncomingData();
		void HandleReadAsync(const boost::system::error_code&, size_t bytesTransferred);

		boostTCP::socket m_socket;
		char m_readBuffer[128];
		bool m_isConnected;
	};
}