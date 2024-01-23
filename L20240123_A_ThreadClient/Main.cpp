#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include <WinSock2.h>
#include <Windows.h>
#include <process.h>
#include <iostream>


#pragma comment(lib,"ws2_32")

SOCKET ServerSocket;

using namespace std;

unsigned WINAPI SendThread(void* Arg)
{
	char Message[1024] = { 0, };
	char Loop1[1024] = "__))__\n";
	char Loop2[1024] = "__((__\n";
	while (true)
	{
		cin.getline(Message,sizeof(Message));

		int SendLength = send(ServerSocket, Message, (int)strlen(Message), 0);
		for (int i = 0; i < 1000000; i++)
		{
			send(ServerSocket, Loop1, (int)strlen(Loop1), 0);
			send(ServerSocket, Loop2, (int)strlen(Loop2), 0);
			if (i ==50000)
			{
				Sleep(1);
				i = 0;
			}
		}
		
	}
	
	return 0;
}

unsigned WINAPI RecvThread(void* Arg)
{
	char Buffer[1024] = { 0, };
	while (true)
	{
		int RecvData = recv(ServerSocket, Buffer, sizeof(Buffer), 0);

		std::cout << Buffer << std::endl;
	}

	
	return 0;
}
int main()
{


	WSAData wsaData;
	WSAStartup(MAKEWORD(2, 2), &wsaData);

	ServerSocket = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);

	SOCKADDR_IN ServerSockAddr;
	memset(&ServerSockAddr, 0, sizeof(ServerSockAddr));

	ServerSockAddr.sin_family = AF_INET;
	ServerSockAddr.sin_addr.s_addr = inet_addr("192.168.3.118");//192.168.3.118
	ServerSockAddr.sin_port = htons(22222);

	connect(ServerSocket, (SOCKADDR*)&ServerSockAddr, sizeof(ServerSockAddr));

	HANDLE ThreadHandles[2];
	ThreadHandles[0] = (HANDLE)_beginthreadex(0, 0, SendThread, 0, 0, 0);
	ThreadHandles[1] = (HANDLE)_beginthreadex(0, 0, RecvThread, 0, 0, 0);

	while (true)
	{
		Sleep(1);
	}

	closesocket(ServerSocket);

	WSACleanup();
	return 0;
}
