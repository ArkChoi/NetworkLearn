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
	SOCKADDR_IN ServerSockAddr; //ListenSocket 정보
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

	short FileCount = 0;
	int RecvBytes = recvfrom(ServerSocket, (char*)&FileCount, (int)sizeof(FileCount), 0, (SOCKADDR*)&RemoteSockAddr, &ClientSockAddrLenght);
	FileCount = ntohs(FileCount);

	for (int i = 0; i < FileCount; ++i)
	{
		char Buffer[1024] = { 0, };
		short FilenameSize = 0;
		
		//파일 이름
		RecvBytes = recvfrom(ServerSocket, Buffer, sizeof(Buffer), 0, (SOCKADDR*)&RemoteSockAddr, &ClientSockAddrLenght);

		FILE* OutputFile = fopen(Buffer, "wb");

		int FileSize = 0;
		RecvBytes = recvfrom(ServerSocket, (char*)&FileSize, sizeof(FileSize), 0, (SOCKADDR*)&RemoteSockAddr, &ClientSockAddrLenght);
		FileSize = ntohl(FileSize);

		int TotalRecvByte = 0;
		do
		{
			RecvBytes = recvfrom(ServerSocket, Buffer, sizeof(Buffer), 0, (SOCKADDR*)&RemoteSockAddr, &ClientSockAddrLenght);
			//cout << RecvBytes << endl; //이거 넣으면 파일이 유실됨..
			TotalRecvByte += RecvBytes;
			if (RecvBytes <= 0)
			{
				break;
			}
			size_t WriteSize = fwrite(Buffer, sizeof(char), RecvBytes, OutputFile);
		} while (FileSize != TotalRecvByte);
		fclose(OutputFile);

	}

	closesocket(ServerSocket);

	WSACleanup();

	return 0;
}