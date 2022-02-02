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
	UDPBase();
	virtual ~UDPBase() = default;

	UDPBase(UDPBase&&) = delete;
	UDPBase(const UDPBase&) = delete;
	UDPBase& operator=(UDPBase&&) = delete;
	UDPBase& operator=(const UDPBase&) = delete;

	virtual bool Initialize() = 0;
	virtual void Run() = 0;

	[[nodiscard]] TransferData GetData() const { return m_TransferData[0]; }

protected:
	asio::io_context m_IoContext;

	TransferData m_TransferData[1];
};

class UDPServer final : public UDPBase
{
public:
	UDPServer();
	~UDPServer() override;

	[[nodiscard]] virtual bool Initialize() override;
	virtual void Run() override;


private:
	udp::socket m_Socket;
	udp::endpoint m_Client;
};

class UDPClient final : public UDPBase
{
public:
	UDPClient();
	~UDPClient() override;

	[[nodiscard]] virtual bool Initialize() override;
	virtual void Run() override;

private:
	udp::socket m_Socket;
	udp::resolver m_Resolver;
	udp::endpoint m_Host;
};