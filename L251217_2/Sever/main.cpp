#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <string>
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

	//IP v4, Steram 타입의 소켓 만들어 주셈, TCP
	//문지기
	SOCKET ListenSocket = socket(AF_INET, SOCK_STREAM, 0);

	SOCKADDR_IN ServerSockAddr;
	memset(&ServerSockAddr, 0, sizeof(ServerSockAddr));
	//ZeroMemory(&ServerSockAddr, sizeof(ServerSockAddr));
	ServerSockAddr.sin_family = PF_INET;
	ServerSockAddr.sin_addr.s_addr = INADDR_ANY; //수업용
	ServerSockAddr.sin_port = htons(777); //Byte Order

	bind(ListenSocket, (SOCKADDR*)&ServerSockAddr, sizeof(ServerSockAddr));

	listen(ListenSocket, 5);

	SOCKADDR_IN ClientSockAddr;
	memset(&ClientSockAddr, 0, sizeof(ClientSockAddr));
	int ClientSockAddrSize = sizeof(ClientSockAddr);

	while (true)
	{
		SOCKET ClientSocket = accept(ListenSocket, (SOCKADDR*)&ClientSockAddr, &ClientSockAddrSize);

		Data Buffer;

		int RecvByte = recv(ClientSocket, (char*)&Buffer, sizeof(Buffer), 0);

		Result ResultBuffer;
		ResultBuffer.Number = Buffer.Number1 + Buffer.Number2;

		int SentByte = send(ClientSocket, (char*)&ResultBuffer, sizeof(ResultBuffer), 0);

		closesocket(ClientSocket);
	}

	closesocket(ListenSocket);

	WSACleanup();

	return 0;
}