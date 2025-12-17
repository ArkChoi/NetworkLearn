#define _CRT_SECURE_NO_WARNINGS
#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include <iostream>
#include <string>
#include <random>
#include <WinSock2.h>

#pragma comment(lib, "ws2_32")

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

		//c언어 스타일 문자열 처리,
		//c++ string
		//std::default_random_engine generator((unsigned int)std::time(0));
		//std::uniform_int_distribution<int>  distribution(10, 99);
		//int Number1 = distribution(generator);
		//int Number2 = distribution(generator);

		//std::string Buffer;
		//Buffer = std::to_string(Number1) + "+" + std::to_string(Number2);
		//std::cout << Buffer << std::endl;

		int Number1 = rand() % 90 + 10;
		int Number2 = rand() % 90 + 10;

		char Buffer[1024] = { 0, };
		sprintf(Buffer, "%d+%d", Number1, Number2);

		std::cout << Buffer << std::endl;

		//[1][0][+][1][0][\0][][]
		int SentByte = send(ServerSocket, Buffer, (int)strlen(Buffer), 0);

		int RecvByte = recv(ServerSocket, Buffer, sizeof(Buffer) - 1, 0);
		std::cout << Buffer << std::endl;


		closesocket(ServerSocket);
	}

	WSACleanup();

	return 0;
}