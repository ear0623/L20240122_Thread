
#include <WinSock2.h>
#include <iostream>

#pragma comment(lib,"ws2_32")

int main()
{
	WSAData wsaData;
	WSAStartup(MAKEWORD(2, 2), &wsaData);

	SOCKET ListenSocket = socket(PF_INET,SOCK_STREAM,IPPROTO_TCP); 

	SOCKADDR_IN ListenSockAddr;
	memset(&ListenSockAddr, 0, sizeof(ListenSockAddr));

	ListenSockAddr.sin_family = AF_INET;
	ListenSockAddr.sin_addr.s_addr = INADDR_ANY;//받을 IP config파일로 만들어서 왔다리갔다리
	ListenSockAddr.sin_port = htons(15689);
	
	bind(ListenSocket, (SOCKADDR*)&ListenSockAddr, sizeof(ListenSockAddr));

	listen(ListenSocket, 5);

	fd_set ReadSocketList;
	fd_set CopyReadSocketList;
	//TSet-> 배열로 정렬 안됨 //set-> 자료구조라서 정렬됨
	FD_ZERO(&ReadSocketList);
	FD_ZERO(&CopyReadSocketList);

	FD_SET(ListenSocket,&ReadSocketList);//감시할 소켓을 넣어줘
	struct timeval Time;
	Time.tv_sec = 0;
	Time.tv_usec = 100;

	while (true)
	{
		CopyReadSocketList = ReadSocketList;
		int SocketEventCount = select(0, &CopyReadSocketList, NULL, NULL, &Time);//socket 바뀐거 있나요 os님 ㅍ
		//싱글스레드
		if (SocketEventCount == 0)//select 반복작업,하나로 그래서 스레드?
		{
			//error or null
			continue;
		}
		else
		{
			for (int i = 0; i < (int)ReadSocketList.fd_count; i++)
			{//포함되어있니 isset
				
				if (FD_ISSET(ReadSocketList.fd_array[i], &CopyReadSocketList))
				{
					if (ReadSocketList.fd_array[i] == ListenSocket)//socket
					{
						SOCKADDR_IN ClientSockAddr;
						memset(&ClientSockAddr, 0, sizeof(ClientSockAddr));
						int ClientSockAddrLenth = sizeof(ClientSockAddr);
						SOCKET ClientSocket = accept(ListenSocket, (SOCKADDR*)&ClientSockAddr, &ClientSockAddrLenth);// 메모리에 넣어주고 주소값만 주는 시스템
						FD_SET(ClientSocket, &ReadSocketList);

					}
					else//data
					{
						char Buffer[1024] = { 0, };
						int RecvData = recv(ReadSocketList.fd_array[i], Buffer, 1024, 0);
						
						if (RecvData <= 0)
						{
							//null or error
							closesocket(ReadSocketList.fd_array[i]);
							FD_CLR(ReadSocketList.fd_array[i], &ReadSocketList);
							
						}
						else
						{
							std::cout << Buffer << std::endl;
							for (int j = 0; j < (int)ReadSocketList.fd_count; j++)
							{
								int SendLength = send(ReadSocketList.fd_array[j], Buffer, sizeof(Buffer), 0);//모자른만큼 보내줘야
							}
							
						}
					}
				}
			}
		}
	}

	closesocket(ListenSocket);
	
	WSACleanup();


	return 0;
}