#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include <WinSock2.h>
#include <iostream>

#pragma comment(lib,"ws2_32")

using namespace std;

int main()
{
	WSAData wsaData;
	WSAStartup(MAKEWORD(2, 2), &wsaData);

	SOCKADDR_IN ServerSockAddr;
	memset(&ServerSockAddr, 0, sizeof(ServerSockAddr));
	ServerSockAddr.sin_family = AF_INET;
	ServerSockAddr.sin_addr.s_addr = inet_addr("127.0.0.1");//ø¨∞·«“ IP
	ServerSockAddr.sin_port = htons(15689);
	SOCKET ServerSocket=socket(PF_INET,SOCK_STREAM,IPPROTO_TCP);

	int ServerSockAddrLenth = sizeof(ServerSockAddr);

	connect(ServerSocket, (SOCKADDR*)&ServerSockAddr, ServerSockAddrLenth);
	while (true)
	{
		
		char Message[1024] = { 0, };
		for (int i = 0; i < 1; i++)
		{
			cin >> Message;
		}
		printf_s(Message);
		send(ServerSocket, Message, sizeof(Message), 0);
		std::cout << Message << std::endl;

		char Buffer[1024] = { 0, };

		int RecvLen = recv(ServerSocket, Buffer, sizeof(Buffer), 0);
		if (RecvLen <= 0)
		{
			//null or error
		}
		else
		{
			//vaild Data
		}
	}
	closesocket(ServerSocket);

	WSACleanup();

	return 0;
}