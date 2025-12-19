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

	bind(ServerSocket, (SOCKADDR*)&ServerSockAddr, sizeof(ServerSockAddr));

	char Buffer[4096] = { 0, };

	SOCKADDR_IN ClientSockAddr;
	memset(&ClientSockAddr, 0, sizeof(ClientSockAddr));
	int ClientSockAddrLength = sizeof(ClientSockAddr);

	vector<string> FileList;

	FileList.push_back("tree.jpg");
	FileList.push_back("baddong.jpg");

	int RecvBytes = recvfrom(ServerSocket, Buffer, sizeof(Buffer), 0, (SOCKADDR*)&ClientSockAddr, &ClientSockAddrLength);
	if (RecvBytes <= 0)
	{
		exit(-1);
	}
	else
	{
		if (strcmp(Buffer, "내놔") != 0)
		{
			exit(-1);
		}
	}

	// server <- 내놔 그담부터 전송
	// 파일 이름보내주고 (packet 길이)
	// 파일내용 보내줌 (분할 해서 보낼꺼니깐)
	// 파일끝, 전체끝
	for (int i = 0; i < FileList.size(); ++i)
	{
		//파일 이름 전송
		int SendBytes = sendto(ServerSocket, FileList[i].c_str(), (int)FileList[i].length(), 0, (SOCKADDR*)&ClientSockAddr, sizeof(ClientSockAddr));

		//파일 읽기 
		FILE* InputFile = fopen(FileList[i].c_str(), "rb");
		//파일 내용
		do
		{
			size_t ReadSize = fread(Buffer, sizeof(char), (int)sizeof(Buffer), InputFile);

			SendBytes = sendto(ServerSocket, Buffer, (int)ReadSize, 0, (SOCKADDR*)&ClientSockAddr, sizeof(ClientSockAddr));

			//Sleep(1);
			//cout << ReadSize << endl;
			if (SendBytes <= 0)
			{
				//break;
			}
		} while (!feof(InputFile));

		string EndMessage = "파일끝";
		SendBytes = sendto(ServerSocket, EndMessage.c_str(), (int)EndMessage.length() + 1, 0, (SOCKADDR*)&ClientSockAddr, sizeof(ClientSockAddr));

		fclose(InputFile);
	}

	string EndMessage = "전체끝";
	int SendBytes = sendto(ServerSocket, EndMessage.c_str(), (int)EndMessage.length() + 1, 0, (SOCKADDR*)&ClientSockAddr, sizeof(ClientSockAddr));


	closesocket(ServerSocket);

	WSACleanup();

	return 0;
}