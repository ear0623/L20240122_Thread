#include <WinSock2.h>

#pragma comment(lib,"ws2_32")

int main()
{	
	WSAData wsaData;
	WSAStartup(MAKEWORD(2,2),&wsaData);

 	SOCKET ListenSocket = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);

	SOCKADDR_IN ListenSocketAddr;//bth-> 블루투스
	memset(&ListenSocketAddr, 0, sizeof(ListenSocketAddr));
	ListenSocketAddr.sin_family = AF_INET;
	ListenSocketAddr.sin_addr.s_addr = INADDR_ANY;//inet_addr("127.0.0.1");
	ListenSocketAddr.sin_port = htons(10880);
	bind(ListenSocket, (struct sockaddr*) & ListenSocketAddr,sizeof(ListenSocketAddr));

	listen(ListenSocket, 5);


	fd_set ReadSocketLiset;//선언
	fd_set CopyReadSocketLiset;
	FD_ZERO(&ReadSocketLiset);//초기화
	FD_ZERO(&CopyReadSocketLiset);
	FD_SET(ListenSocket, &ReadSocketLiset);//read에 listen을 넣어준다.
	struct timeval TimeOut;
	TimeOut.tv_sec = 0;
	TimeOut.tv_usec = 10;//기다리라

	while (true)
	{
		int SocketEventCount = select(0, &CopyReadSocketLiset, nullptr, nullptr, &TimeOut);//os님에게 전달
		if (SocketEventCount == 0)
		{
			//other process
			continue;
		}
		else if (SocketEventCount < 0)
		{
			//error
		}
		for (int i = 0; i < (int)ReadSocketLiset.fd_count; i++)
		{
			if (FD_ISSET(ReadSocketLiset.fd_array[0], &CopyReadSocketLiset))
			{
				if (ReadSocketLiset.fd_array[i] == ListenSocket)
				{
					SOCKADDR_IN ClientSocketAddr;//bth-> 블루투스
					memset(&ClientSocketAddr, 0, sizeof(ClientSocketAddr));
					int ClientSockaetAddrSize = sizeof(ClientSocketAddr);
					SOCKET ClientSocket = accept(ListenSocket, (struct sockaddr*)&ClientSocketAddr, &ClientSockaetAddrSize);
					FD_SET(ClientSocket, &ReadSocketLiset);
				}
				else//data 처리
				{
					char Buffer[1024] = { 0, };//os님한테 달라한거임
					int RecvLegth = recv(ReadSocketLiset.fd_array[i], Buffer, 1024, 0);
					if (RecvLegth <= 0)
					{
						//exit
						closesocket(ReadSocketLiset.fd_array[i]);
						FD_CLR(ReadSocketLiset.fd_array[i], &ReadSocketLiset);

					}
					else
					{
						int SendLength = send(ReadSocketLiset.fd_array[i], Buffer, RecvLegth, 0);
						//2로 들어올 수도 있어서 한번에 못보낼 수도 있어서
						if (SendLength <= 0)
						{

						}
						else
						{

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