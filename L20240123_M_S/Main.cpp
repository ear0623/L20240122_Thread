
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
	ListenSockAddr.sin_addr.s_addr = INADDR_ANY;//���� IP config���Ϸ� ���� �Դٸ����ٸ�
	ListenSockAddr.sin_port = htons(15689);
	
	bind(ListenSocket, (SOCKADDR*)&ListenSockAddr, sizeof(ListenSockAddr));

	listen(ListenSocket, 5);

	fd_set ReadSocketList;
	fd_set CopyReadSocketList;
	//TSet-> �迭�� ���� �ȵ� //set-> �ڷᱸ���� ���ĵ�
	FD_ZERO(&ReadSocketList);
	FD_ZERO(&CopyReadSocketList);

	FD_SET(ListenSocket,&ReadSocketList);//������ ������ �־���
	struct timeval Time;
	Time.tv_sec = 0;
	Time.tv_usec = 100;

	while (true)
	{
		CopyReadSocketList = ReadSocketList;
		int SocketEventCount = select(0, &CopyReadSocketList, NULL, NULL, &Time);//socket �ٲ�� �ֳ��� os�� ��
		//�̱۽�����
		if (SocketEventCount == 0)//select �ݺ��۾�,�ϳ��� �׷��� ������?
		{
			//error or null
			continue;
		}
		else
		{
			for (int i = 0; i < (int)ReadSocketList.fd_count; i++)
			{//���ԵǾ��ִ� isset
				
				if (FD_ISSET(ReadSocketList.fd_array[i], &CopyReadSocketList))
				{
					if (ReadSocketList.fd_array[i] == ListenSocket)//socket
					{
						SOCKADDR_IN ClientSockAddr;
						memset(&ClientSockAddr, 0, sizeof(ClientSockAddr));
						int ClientSockAddrLenth = sizeof(ClientSockAddr);
						SOCKET ClientSocket = accept(ListenSocket, (SOCKADDR*)&ClientSockAddr, &ClientSockAddrLenth);// �޸𸮿� �־��ְ� �ּҰ��� �ִ� �ý���
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
								int SendLength = send(ReadSocketList.fd_array[j], Buffer, sizeof(Buffer), 0);//���ڸ���ŭ �������
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