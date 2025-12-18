#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS


#include <winsock2.h>
#include <iostream>
#include <string>
#include <vector>

#pragma comment(lib, "ws2_32")

using namespace std;

//server.exe 127.0.0.1
int main()
{
	WSAData wsaData;

	WSAStartup(MAKEWORD(2, 2), &wsaData);

	SOCKET ServerSocket = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP);
	SOCKADDR_IN ServerSockAddr; //ListenSocket Á¤º¸
	memset(&ServerSockAddr, 0, sizeof(ServerSockAddr));
	ServerSockAddr.sin_family = AF_INET;
	ServerSockAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
	ServerSockAddr.sin_port = htons(33333);

	bind(ServerSocket, (SOCKADDR*)&ServerSockAddr, sizeof(ServerSockAddr));

	char Buffer[1024] = "Hello World UDP";

	int SendBytes = sendto(ServerSocket, Buffer, (int)(strlen(Buffer)+1), 0, (SOCKADDR*)&ServerSockAddr, sizeof(ServerSockAddr));

	if (SendBytes <= 0)
	{

	}

	SOCKADDR_IN RemoteSockAddr; 
	memset(&RemoteSockAddr, 0, sizeof(RemoteSockAddr));
	int ClientSockAddrLenght = sizeof(RemoteSockAddr);

	int RecvBytes = recvfrom(ServerSocket, Buffer, sizeof(Buffer), 0, (SOCKADDR*)&RemoteSockAddr, &ClientSockAddrLenght);
	if (RecvBytes <= 0)
	{

	}

	cout << Buffer << endl;

	closesocket(ServerSocket);

	WSACleanup();

	return 0;
}