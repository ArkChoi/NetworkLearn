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
	vector<string> FileList;

	FileList.push_back("tree.jpg");
	FileList.push_back("baddong.jpg");
	FileList.push_back("말안듣는동준이.jpg");

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

	short FileCount = (short)FileList.size();
	FileCount = htons(FileCount); //바이트오더 변경(네트워크 바이트오더)
	int SentBytes = send(ClientSocket, (char*)&FileCount, sizeof(FileCount), 0);

	for (int i = 0; i < FileList.size(); ++i)
	{
		//파일 이름 길이 전송
		short FilenameSize = (short)FileList[i].length();
		FilenameSize = htons(FilenameSize); //바이트오더 변경(네트워크 바이트오더)
		SentBytes = send(ClientSocket, (char*)&FilenameSize, sizeof(FilenameSize), 0);

		//파일 이름 전송
		SentBytes = send(ClientSocket, FileList[i].c_str(), (int)FileList[i].length(), 0);

		//파일 읽기 
		FILE* InputFile = fopen(FileList[i].c_str(), "rb");
		fseek(InputFile, 0, SEEK_END);
		int FileSize = ftell(InputFile);
		fseek(InputFile, 0, SEEK_SET);

		int NetworkByteOrder = htonl(FileSize);

		//파일 크기 전송
		int SentBytes = send(ClientSocket, (char*)&NetworkByteOrder, sizeof(NetworkByteOrder), 0);

		//파일 내용
		do
		{
			size_t ReadSize = fread(Buffer, sizeof(char), (int)sizeof(Buffer), InputFile);

			int SentBytes = send(ClientSocket, Buffer, (int)ReadSize, 0);
			if (SentBytes <= 0)
			{
				break;
			}
		} while (!feof(InputFile));

		fclose(InputFile);
	}


	closesocket(ClientSocket);
	closesocket(ListenSocket);

	WSACleanup();

	return 0;
}