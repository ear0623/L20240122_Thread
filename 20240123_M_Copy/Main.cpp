#define _WINSOCK_DEPRECATED_NO_WARNINGS 

#include <WinSock2.h>
#include <iostream>

#pragma comment(lib, "ws2_32")

int main()
{
	WSAData wsaData;
	WSAStartup(MAKEWORD(2, 2), &wsaData);

	SOCKADDR_IN ServerSockAddr;
	memset(&ServerSockAddr, 0, sizeof(ServerSockAddr));
	ServerSockAddr.sin_family = AF_INET;
	ServerSockAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
	ServerSockAddr.sin_port = htons(10880);

	SOCKET ServerSocket = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);

	connect(ServerSocket, (SOCKADDR*)&ServerSockAddr, sizeof(ServerSockAddr));
	while (true)
	{
		char Buffer[1024] = "Hello World";
		sprintf_s(Buffer, "Hello World %d", rand());
		int SendLength = send(ServerSocket, Buffer, (int)strlen(Buffer), 0);
		char Message[1024] = { 0, };
		recv(ServerSocket, Message, sizeof(Message), 0);

		std::cout << Message << std::endl;
	}

	WSACleanup();

	return 0;
}