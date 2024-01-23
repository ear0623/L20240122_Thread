#include <iostream>
#include <WinSock2.h>
#include <WS2tcpip.h>

#pragma comment(lib,"ws2_32")

using namespace std;

int main()
{
	WSAData WsaData;
	WSAStartup(MAKEWORD(2, 2), &WsaData);

	SOCKET ServerSocket = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
	struct sockaddr_in ServerSockAddr;
	memset(&ServerSockAddr, 0, sizeof(ServerSockAddr));
	ServerSockAddr.sin_family = AF_INET;
	inet_pton(AF_INET, "127.0.0.1", &(ServerSockAddr.sin_addr.s_addr));
	ServerSockAddr.sin_port = htons(10880);
	//192.168.3.118
	connect(ServerSocket, (struct sockaddr*)&ServerSockAddr, sizeof(ServerSockAddr));

	while (true)
	{
		char Message[1024] = { 0, };
		for (int i = 0; i < 10; i++)
		{
			Message[i] = 'r';
		};
		int RecvData = send(ServerSocket, Message,(u_int)sizeof(Message),0);
		if (RecvData == 0)
		{
			//nullptr
		}
		else if (RecvData < 0)
		{
			//error
		}
		else
		{
			//process
		}

		char DataBuffer[1024] = { 0, };
		int RecvByte = recv(ServerSocket, DataBuffer, 1024, 0);
		if (RecvByte == 0)
		{
			//nullptr
		}
		else if (RecvByte < 0)
		{
			//error
		}
		else
		{
			//process
		}
	}

	WSACleanup();
}