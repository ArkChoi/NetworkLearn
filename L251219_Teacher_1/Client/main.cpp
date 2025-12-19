#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS


#include <winsock2.h>
#include <iostream>
#include <string>
#include <vector>

#pragma comment(lib, "ws2_32")

using namespace std;

int main()
{
	WSAData wsaData;

	WSAStartup(MAKEWORD(2, 2), &wsaData);

	SOCKET ServerSocket = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP);
	SOCKADDR_IN ServerSockAddr;
	memset(&ServerSockAddr, 0, sizeof(ServerSockAddr));
	ServerSockAddr.sin_family = AF_INET;
	ServerSockAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
	ServerSockAddr.sin_port = htons(33333);

	char Buffer[4096] = { 0, };

	SOCKADDR_IN ClientSockAddr;
	memset(&ClientSockAddr, 0, sizeof(ClientSockAddr));
	int ClientSockAddrLength = sizeof(ClientSockAddr);


	string EndMessage = "내놔";
	int SendBytes = sendto(ServerSocket, EndMessage.c_str(), (int)EndMessage.length() + 1, 0, (SOCKADDR*)&ServerSockAddr, sizeof(ServerSockAddr));

	while (true)
	{
		//파일 이름
		int RecvBytes = recvfrom(ServerSocket, Buffer, sizeof(Buffer), 0, (SOCKADDR*)&ClientSockAddr, &ClientSockAddrLength);
		if (RecvBytes <= 0)
		{
			break;
		}

		if (strcmp(Buffer, "전체끝") == 0)
		{
			cout << Buffer << endl;
			break;
		}

		FILE* OutputFile = fopen(Buffer, "wb");

		//파일 내용
		do
		{
			memset(&Buffer, 0, sizeof(Buffer));

			int RecvBytes = recvfrom(ServerSocket, Buffer, sizeof(Buffer), 0, (SOCKADDR*)&ClientSockAddr, &ClientSockAddrLength);

			if (RecvBytes <= 0)
			{
				break;
			}
			if (strcmp(Buffer, "파일끝") != 0)
			{
				//cout << RecvBytes << endl; //이거 넣으면 파일이 유실됨..
				//io가 느리기에.. -> ???, 스레드, 이벤트 스레드, IOCP 이렇게 발전되는 거다.
				size_t WriteSize = fwrite(Buffer, sizeof(char), RecvBytes, OutputFile);
			}


		} while (strcmp(Buffer, "파일끝") != 0);

		fclose(OutputFile);
	}

	closesocket(ServerSocket);

	WSACleanup();

	return 0;
}