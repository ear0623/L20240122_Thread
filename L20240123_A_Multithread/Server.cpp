#include <WinSock2.h>
#include <Windows.h>
#include <process.h>
#include <iostream>
#include <vector>
#include <set>
#include <map>

#pragma comment(lib,"ws2_32")

std::set<SOCKET> SessionList;

CRITICAL_SECTION SessionCS;

using namespace std;

unsigned WINAPI WorkerThread(void* Arg)
{
	SOCKET ClientSocket = *((SOCKET*)Arg);//������ ���������̰� �����ͷ� �ٲ㷪
	
	while (true)
	{
		char Buffer[1024] = { 0, };
		int RecvData = recv(ClientSocket, Buffer, 1024, 0);
		if (RecvData <= 0)
		{
			//null or error
			EnterCriticalSection(&SessionCS);
			SessionList.erase(ClientSocket);
			LeaveCriticalSection(&SessionCS);
			closesocket(ClientSocket);
			//ExitThread(-1);
			break;
		
		}
		else
		{
			cout << RecvData << endl;
			EnterCriticalSection(&SessionCS);
			for (auto connectSocket : SessionList)
			{
				int SendLength = send(connectSocket, Buffer, RecvData, 0);//���ڸ���ŭ �������
			}
			LeaveCriticalSection(&SessionCS);
		
		}
	}
	return 0;//thrad ���� -> �����޸�
}

int main()
{
	InitializeCriticalSection(&SessionCS);

	WSAData wsaData;
	WSAStartup(MAKEWORD(2, 2), &wsaData);

	SOCKET ListenSocket = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);

	SOCKADDR_IN ListenSockAddr;
	memset(&ListenSockAddr, 0, sizeof(ListenSockAddr));

	ListenSockAddr.sin_family = AF_INET;
	ListenSockAddr.sin_addr.s_addr = INADDR_ANY;//���� IP config���Ϸ� ���� �Դٸ����ٸ�
	ListenSockAddr.sin_port = htons(15689);

	bind(ListenSocket, (SOCKADDR*)&ListenSockAddr, sizeof(ListenSockAddr));

	listen(ListenSocket, 5);

	

	while (true)
	{
		SOCKADDR_IN ClientSockAddr;
		memset(&ClientSockAddr, 0, sizeof(ClientSockAddr));
		int ClientSockAddrLenth = sizeof(ClientSockAddr);
		SOCKET ClientSocket = accept(ListenSocket, (SOCKADDR*)&ClientSockAddr, &ClientSockAddrLenth);// �޸𸮿� �־��ְ� �ּҰ��� �ִ� �ý���
		//thread���� worker thread// ����ȭ�ȵǼ�cout printf
		HANDLE ThreadHandle = (HANDLE)_beginthreadex(0, 0, WorkerThread, (void*)&ClientSocket, 0, 0);
		EnterCriticalSection(&SessionCS);
		SessionList.insert(ClientSocket);
		LeaveCriticalSection(&SessionCS);
		
		//Terminate(ThreadHandle,-1)

	}

	closesocket(ListenSocket);

	WSACleanup();

	DeleteCriticalSection(&SessionCS);
	return 0;
}