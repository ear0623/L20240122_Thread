#define FD_SETSIZE 100
#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include <iostream>
#include <WinSock2.h>

using namespace std;

#pragma comment(lib,"ws2_32")

int main()
{
	WSAData wsaData;
	WSAStartup(MAKEWORD(2, 2), &wsaData);

	SOCKET ListenSocket = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);

	SOCKADDR_IN ListenSocketAddr;
	memset(&ListenSocketAddr, 0, sizeof(ListenSocketAddr));
	ListenSocketAddr.sin_family = AF_INET;
	ListenSocketAddr.sin_addr.s_addr = INADDR_ANY;
	ListenSocketAddr.sin_port = htons(10880);

	bind(ListenSocket, (SOCKADDR*)&ListenSocketAddr, sizeof(ListenSocketAddr));

	listen(ListenSocket, 5);//����

	fd_set ReadSocketList;//����
	FD_ZERO(&ReadSocketList);//�ʱ�ȭ

	FD_SET(ListenSocket, &ReadSocketList);//�������������� ���ϼ���

	struct timeval  TimeOut;
	TimeOut.tv_sec = 0;
	TimeOut.tv_usec = 100;

	fd_set CopySocketLists;
	FD_ZERO(&CopySocketLists);

	//timeout Ŭ���� �����ɸ� . 
	while (true)
	{
		CopySocketLists = ReadSocketList;
		//polling->upgrade epoll//iocp �� windows
		int EventSocketCount = select(0, &CopySocketLists, nullptr, nullptr, &TimeOut);//in out ����ü�� ī��
		if (EventSocketCount == 0)
		{
			//no event;
			//cout << "���߳�" << endl;
		}
		else if (EventSocketCount < 0)
		{
			//error
			
		}
		else
		{
			//process
			for (int i = 0; i < (int)ReadSocketList.fd_count; i++)
			{
				if (FD_ISSET(ReadSocketList.fd_array[i], &CopySocketLists))//����
				{
					if (ReadSocketList.fd_array[i] == ListenSocket)
					{
						

						SOCKADDR_IN ClientSocketAddr;
						memset(&ClientSocketAddr, 0, sizeof(ClientSocketAddr));
						int clientAddrlenth = sizeof(ClientSocketAddr);
						SOCKET ClientSocket = accept(ReadSocketList.fd_array[i],(SOCKADDR*) & ClientSocketAddr, &clientAddrlenth);
						FD_SET(ClientSocket, &ReadSocketList);
						cout << "cnnect client :" << inet_ntoa(ClientSocketAddr.sin_addr) << endl;
					}
					else
					{
						//recv
						char Buffer[1024] = { 0, };
						int RecvLength = recv(ReadSocketList.fd_array[i],Buffer,1024,0);
						if (RecvLength == 0)
						{
							FD_CLR(ReadSocketList.fd_array[i], &ReadSocketList);
							cout << "disconnect client :" << ReadSocketList.fd_array[i] << endl;
						}
						else if (RecvLength < 0)
						{
							cout << "error client :" << ReadSocketList.fd_array[i] << endl;
						}
						else
						{
							  
						}
						cout << "recv client :" << ReadSocketList.fd_array[i] << " " << Buffer << endl;
						send(ReadSocketList.fd_array[i], Buffer, RecvLength, 0);
					}
				}
			}
		}
	}
	

	WSACleanup();

	return 0;
}
//int Number = 0xABCD1234;
	////���ڸ�
	//printf("0x%x", Number);
	//printf("0x%x", htonl(Number));
	//printf("0x%x", ntohl(htonl(Number)));