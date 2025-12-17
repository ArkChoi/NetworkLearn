#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <string>
#include <WinSock2.h>

#pragma comment(lib, "ws2_32")

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

		char Buffer[1024] = { 0, };

		int RecvByte = recv(ClientSocket, Buffer, sizeof(Buffer), 0);
		if (RecvByte <= 0)
		{
			break;
		}
		//[1][0][+][1][0][\0]
		std::string Message(Buffer);
		std::string Number1 = Message.substr(0, 2);
		std::string Operator = Message.substr(2, 1);
		std::string Number2 = Message.substr(3, 2);

		std::cout << Message << std::endl;
		std::cout << Number1 << std::endl;
		std::cout << Operator << std::endl;
		std::cout << Number2 << std::endl;

		int Result = std::stoi(Number1) + std::stoi(Number2);
		sprintf(Buffer, "%d", Result);

		int SentByte = send(ClientSocket, Buffer, strlen(Buffer) + 1, 0);

		closesocket(ClientSocket);
	}

	closesocket(ListenSocket);

	WSACleanup();

	return 0;
}