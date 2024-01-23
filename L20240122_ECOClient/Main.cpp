#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include <WinSock2.h>
#include <iostream>

#pragma comment(lib,"ws2_32")

int main()
{
	WSAData wsaData;
	WSAStartup(MAKEWORD(2, 2), &wsaData);
	
	
		SOCKADDR_IN ServersocketAddr;
		memset(&ServersocketAddr, 0, sizeof(ServersocketAddr));
		ServersocketAddr.sin_family = AF_INET;
		ServersocketAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
		ServersocketAddr.sin_port = htons(10880);
		//192.168.3.118
		SOCKET ServerSocket = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);

		connect(ServerSocket, (SOCKADDR*)&ServersocketAddr, sizeof(ServersocketAddr));
		//os에 가까운 버전의 os를 사용하면 더 좋음
	while (true)
	{
		char Buffer[1024] = "Hellow";//1kb
		//sprintf_s(Buffer,"Hllow World %d")
		int SendLength = send(ServerSocket, Buffer, (int)strlen(Buffer), 0);
		if (SendLength == 0)
		{

		}
		else if (SendLength < 0)
		{

		}
		else
		{

		}
		char Message[1024] = { 0, };
		int RecvLength = recv(ServerSocket, Message, sizeof(Message), 0);
		if (RecvLength == 0)
		{

		}
		else if (RecvLength < 0)
		{

		}
		else
		{

		}
		std::cout << Message << std::endl;



	}
	
	WSACleanup();
	return 0;
}