#include "UDPNetwork.h"
#include <iostream>


#pragma region Server
UDPServer::UDPServer()
	: UDPBase()
	, m_Socket(m_IoContext, udp::endpoint(udp::v4(), 13))
{
}

UDPServer::~UDPServer()
{
	m_Socket.close();
}

bool UDPServer::Initialize()
{
	char message[1] = { ' ' };
	udp::endpoint remote_endpoint;
	m_Socket.receive_from(asio::buffer(message), remote_endpoint);
	std::cout << "A Client Connected!\n";

	// set message to indicate if succesfull connected
	message[0] = { '+' };
	m_Socket.send_to(asio::buffer(message), remote_endpoint);

	return true;
}

void UDPServer::Run()
{
	POINT lastPos = { 0, 0 };
	for (;;)
	{
		POINT p;
		if (!GetPhysicalCursorPos(&p))
			continue;

		// check if there was any change
		if (p.x == lastPos.x && p.y == lastPos.y)
			continue;
		
		m_TransferData->x = p.x;
		m_TransferData->y = p.y;

		m_Socket.send_to(asio::buffer(m_TransferData), m_Client);

		lastPos = p;
	}
}
#pragma endregion

#pragma region Client
UdpClient::UdpClient()
	: UDPBase()
	, m_Socket(m_IoContext)
	, m_Resolver( m_IoContext )
	, m_Host(*m_Resolver.resolve(udp::v4(), "192.168.0.172", "CursorMovement").begin())
{
}

UdpClient::~UdpClient()
{
	m_Socket.close();
}

bool UdpClient::Initialize()
{
	m_Socket.open(udp::v4());

	// send first msg to start connection
	char message[1] = { '_' };
	m_Socket.send_to(asio::buffer(message), m_Host);

	// check for an answer
	m_Socket.receive_from(asio::buffer(message), m_Host);

	if (!strcmp(message, "+"))
	{
		std::cout << "No response from Server!\n";
		return false;
	}

	std::cout << "Connected to a Server!\n";
	return true;
}

void UdpClient::Run()
{
	for (;;)
	{
		udp::endpoint senderEndpoint;
		m_Socket.receive_from(asio::buffer(m_TransferData), senderEndpoint);

		SetPhysicalCursorPos(m_TransferData->x, m_TransferData->y);
	}
}
#pragma endregion 
