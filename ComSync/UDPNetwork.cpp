#include "UDPNetwork.h"

#pragma region Server
bool UDPServer::Initialize()
{


	return true;
}

void UDPServer::Run()
{

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

	return strcmp(message, "+");
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
