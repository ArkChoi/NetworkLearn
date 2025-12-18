#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS


#include <winsock2.h>
#include <iostream>

#pragma comment(lib, "ws2_32")

using namespace std;

int main()
{
	WSAData wsaData;

	WSAStartup(MAKEWORD(2, 2), &wsaData);

	SOCKET ServerSocket = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
	SOCKADDR_IN ServerSockAddr; //ListenSocket 정보
	memset(&ServerSockAddr, 0, sizeof(ServerSockAddr));
	ServerSockAddr.sin_family = AF_INET;
	ServerSockAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
	ServerSockAddr.sin_port = htons(33333);

	connect(ServerSocket, (SOCKADDR*)&ServerSockAddr, sizeof(ServerSockAddr));

	//파일 갯수, file이름(길이), 내용(길이)
	// 
	//[][] 파일 갯수
	// 
	//[][] 파일 이름 길이
	//[][][][][] 파일이름
	//[][][][] 파일크기
	//[][][][][] -> 파일내용
	// 
	//[][] 파일 이름 길이
	//[][][][][] 파일이름
	//[][][][] 파일크기
	//[][][][][] -> 파일내용

	//파일갯수
	short FileCount = 0;
	int RecvBytes = recv(ServerSocket, (char*)&FileCount, (int)sizeof(FileCount), MSG_WAITALL);
	FileCount = ntohs(FileCount);

	for (int i = 0; i < FileCount; ++i)
	{
		char Buffer[65536] = { 0, };
		short FilenameSize = 0;
		//파일 이름 크기
		int RecvBytes = recv(ServerSocket, (char*)&FilenameSize, sizeof(FilenameSize), MSG_WAITALL);
		FilenameSize = ntohs(FilenameSize);
		//파일 이름
		RecvBytes = recv(ServerSocket, Buffer, (int)FilenameSize, MSG_WAITALL);

		FILE* OutputFile = fopen(Buffer, "wb");

		int FileSize = 0;
		RecvBytes = recv(ServerSocket, (char*)&FileSize, sizeof(FileSize), MSG_WAITALL);
		FileSize = ntohl(FileSize);

		int TotalRecvByte = 0;
		do
		{
			int LeftFileSize = FileSize - TotalRecvByte;
			int ReadSize = sizeof(Buffer);
			if (LeftFileSize < sizeof(Buffer))
			{
				ReadSize = LeftFileSize;
			}


			RecvBytes = recv(ServerSocket, Buffer, ReadSize, 0);
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