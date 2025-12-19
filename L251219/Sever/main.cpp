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
	SOCKADDR_IN ServerSockAddr;
	memset(&ServerSockAddr, 0, sizeof(ServerSockAddr));
	ServerSockAddr.sin_family = AF_INET;
	ServerSockAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
	ServerSockAddr.sin_port = htons(33333);

	bind(ServerSocket, (SOCKADDR*)&ServerSockAddr, sizeof(ServerSockAddr));

	char Buffer[1024] = { 0, };

	SOCKADDR_IN ClientSockAddr;
	memset(&ClientSockAddr, 0, sizeof(ClientSockAddr));
	int ClientSockAddrLength = sizeof(ClientSockAddr);

	vector<string> FileList;

	FileList.push_back("tree.jpg");
	FileList.push_back("Tempbad.jpg");

	while (true)
	{
		int RecvBytes = recvfrom(ServerSocket, Buffer, sizeof(Buffer), 0, (SOCKADDR*)&ClientSockAddr, &ClientSockAddrLength);
		if (RecvBytes <= 0)
		{
			//break;
		}

		short FileCount = (short)FileList.size();
		FileCount = htons(FileCount); //바이트오더 변경(네트워크 바이트오더)
		int SendBytes = sendto(ServerSocket, (char*)&FileCount, sizeof(short), 0, (SOCKADDR*)&ClientSockAddr, sizeof(ClientSockAddr));
		
		for (int i = 0; i < FileList.size(); ++i)
		{
			//파일 이름 전송
			SendBytes = sendto(ServerSocket, FileList[i].c_str(), (int)FileList[i].length(), 0, (SOCKADDR*)&ClientSockAddr, sizeof(ClientSockAddr));

			//파일 읽기 
			FILE* InputFile = fopen(FileList[i].c_str(), "rb");
			fseek(InputFile, 0, SEEK_END);
			int FileSize = ftell(InputFile);
			fseek(InputFile, 0, SEEK_SET);

			int NetworkByteOrder = htonl(FileSize);

			//파일 크기 전송
			SendBytes = sendto(ServerSocket, (char*)&NetworkByteOrder, sizeof(NetworkByteOrder), 0, (SOCKADDR*)&ClientSockAddr, sizeof(ClientSockAddr));

			//파일 내용
			do
			{
				size_t ReadSize = fread(Buffer, sizeof(char), (int)sizeof(Buffer), InputFile);

				SendBytes = sendto(ServerSocket, Buffer, (int)ReadSize, 0, (SOCKADDR*)&ClientSockAddr, sizeof(ClientSockAddr));
				if (SendBytes <= 0)
				{
					//break;
				}
			} while (!feof(InputFile));

			fclose(InputFile);
		}
	}

	closesocket(ServerSocket);

	WSACleanup();

	return 0;
}