#define _CRT_SECURE_NO_WARNINGS
#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include <iostream>
#include <string>
#include <random>
#include <WinSock2.h>

#pragma comment(lib, "ws2_32")

#pragma pack(push, 1)
struct Data
{
	int Number1;
	int Number2;
	char Operator;
};

struct Result
{
	int Number;
};
#pragma pack(pop)

int main()
{
	WSAData wsaData;

	WSAStartup(MAKEWORD(2, 2), &wsaData);

	srand((unsigned int)time(nullptr));


	while (true)
	{
		//IP v4, Steram 타입의 소켓 만들어 주셈, TCP
		//문지기
		SOCKET ServerSocket = socket(AF_INET, SOCK_STREAM, 0);

		SOCKADDR_IN ServerSockAddr;
		memset(&ServerSockAddr, 0, sizeof(ServerSockAddr));
		//ZeroMemory(&ServerSockAddr, sizeof(ServerSockAddr));
		ServerSockAddr.sin_family = PF_INET;
		ServerSockAddr.sin_addr.s_addr = inet_addr("127.0.0.1"); //수업용
		ServerSockAddr.sin_port = htons(777); //Byte Order

		connect(ServerSocket, (SOCKADDR*)&ServerSockAddr, sizeof(ServerSockAddr));

		Data MyPacket;

		MyPacket.Number1 = rand() % 10000;
		MyPacket.Number2 = rand() % 10000;
		MyPacket.Operator = '+';

		std::cout << MyPacket.Number1 << MyPacket.Operator << MyPacket.Number2 << std::endl;

		int SentByte = send(ServerSocket, (char*)&MyPacket, sizeof(Data), 0);

		Result Buffer;

		int RecvByte = recv(ServerSocket, (char*)&Buffer, sizeof(Buffer), 0);
		std::cout << Buffer.Number << std::endl;


		closesocket(ServerSocket);
	}

	WSACleanup();

	return 0;
}