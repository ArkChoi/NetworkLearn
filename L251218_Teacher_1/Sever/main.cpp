#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS


#include <winsock2.h>
#include <iostream>

#pragma comment(lib, "ws2_32")

using namespace std;

//server.exe 127.0.0.1
int main()
{
	WSAData wsaData;

	WSAStartup(MAKEWORD(2, 2), &wsaData);

	SOCKET ListenSocket = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
	SOCKADDR_IN ListenSockAddr; //ListenSocket 정보
	memset(&ListenSockAddr, 0, sizeof(ListenSockAddr));
	ListenSockAddr.sin_family = AF_INET;
	ListenSockAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
	ListenSockAddr.sin_port = htons(33333);

	bind(ListenSocket, (SOCKADDR*)&ListenSockAddr, sizeof(ListenSockAddr));

	listen(ListenSocket, 5);

	SOCKADDR_IN ClientSockAddr;
	int ClientSockAddrLength = sizeof(ClientSockAddr);

	SOCKET ClientSocket = accept(ListenSocket, (SOCKADDR*)&ClientSockAddr, &ClientSockAddrLength);

	char Buffer[1024] = { 0, };
	//파일 읽기 
	FILE* InputFile = fopen("tree.jpg", "rb");
	fseek(InputFile, 0, SEEK_END);
	int FileSize = ftell(InputFile);
	fseek(InputFile, 0, SEEK_SET);

	int NetworkByteOrder = htonl(FileSize);

	//파일 크기 전송
	int SentBytes = send(ClientSocket, (char*)&NetworkByteOrder, sizeof(NetworkByteOrder), 0);

	//파일 내용
	do
	{
		int ReadSize = fread(Buffer, sizeof(char), 4, InputFile);

		int SentBytes = send(ClientSocket, Buffer, ReadSize, 0);
		if (SentBytes <= 0)
		{
			//break;!
		}
	} while (!feof(InputFile));

	fclose(InputFile);

	closesocket(ClientSocket);
	closesocket(ListenSocket);



	WSACleanup();

	return 0;
}