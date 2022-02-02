#pragma once
#include <asio.hpp>

using asio::ip::udp;

struct TransferData
{
	int x, y;
};

class UDPBase
{
public:
	UDPBase() = default;
	virtual ~UDPBase() = default;

	virtual bool Initialize() = 0;
	virtual void Run() = 0;

protected:
	asio::io_context m_IoContext;

};

class UDPServer final : public UDPBase
{
public:
	UDPServer();
	~UDPServer() override;

	virtual bool Initialize() override;
	virtual void Run() override;


private:
	udp::socket m_Socket;
	udp::endpoint m_Client;

	TransferData m_TransferData[1];
};

class UDPClient final : public UDPBase
{
public:
	UDPClient();
	~UDPClient() override;

	virtual bool Initialize() override;
	virtual void Run() override;

private:
	udp::socket m_Socket;
	udp::resolver m_Resolver;
	udp::endpoint m_Host;

	TransferData m_TransferData[1];
};