#pragma once

#ifdef __PACKET_H__
#define__PACKET_H__

#include <cstring>
#include <WinSock2.h>

enum class EPacketType : unsigned short
{
	C2S_Spawn = 100,
	S2C_Spawn,
	C2S_Login,
	S2C_Login,
	C2S_LogOut,
	S2C_LogOut,
	C2S_Move,
	S2C_Move,

	Max
};
class Player
{
public:
	int X;
	int Y;
	int ID;
};
class PacketManager
{
public:
	static unsigned short Size = 0;
	static Player PlayerData;
	static EPacketType Type;
	//char Buffer[1024];//포인터로하면 더 좋음

	void static MakePacket(char* Buffer)
	{
		//size	type	ID		X		Y
		//[][] [][]	[][][][] [][][][] [][][][]
		unsigned short Data = htons(size);
		memcpy(Buffer[0], &Data, 2)
			Data = htons((unsigned short)(Type));
		memcpy(Buffer[2], &Data, 2);
		int Data2 = htonl(PlayerData.ID);
		memcpy(Buffer[4], &Data2, 4);
		int Data2 = htonl(PlayerData.X);
		memcpy(Buffer[8], &Data2, 4);
		int Data2 = htonl(PlayerData.Y);
		memcpy(Buffer[12], &Data2, 4);
	}
};


#endif  __PACKET_H__
