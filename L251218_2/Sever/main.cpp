#define _CRT_SECURE_NO_WARNINGS
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <iostream>
#include <string.h>
#include <string>
#include <WinSock2.h>

#pragma comment(lib, "ws2_32")

#pragma pack(push, 1)
struct Data
{
	int Number1;
	int Number2;
	char Operator;
};

struct Result
{
	int Number;
};

struct ImageFile
{
	bool bIsFin = false;
	int SendSize = 0;
	char Image[1024] = { 0, };
};
#pragma pack(pop)

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
	ServerSockAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
	ServerSockAddr.sin_port = htons(33333); //Byte Order

	bind(ListenSocket, (SOCKADDR*)&ServerSockAddr, sizeof(ServerSockAddr));

	listen(ListenSocket, 5);

	SOCKADDR_IN ClientSockAddr;
	memset(&ClientSockAddr, 0, sizeof(ClientSockAddr));
	int ClientSockAddrSize = sizeof(ClientSockAddr);

	int FileCount = 2;

	const char* FileNames[2];

	FileNames[0] = "tree.jpg";
	FileNames[1] = "baddong.jpg";

	ImageFile TempFile;

	while (true)
	{
		SOCKET ClientSocket = accept(ListenSocket, (SOCKADDR*)&ClientSockAddr, &ClientSockAddrSize);

		int SentByte = send(ClientSocket, (char*)&FileCount, sizeof(int), 0);

		for (int i = 0; i < FileCount; i++)
		{
			TempFile.bIsFin = false;

			FILE* file = fopen(FileNames[i], "rb");
			if (file == NULL)
			{
				return -1;
			}

			int FileNameSize = strlen(FileNames[i]);
			std::cout << FileNameSize << std::endl;
			SentByte = send(ClientSocket, (char*)&FileNameSize, sizeof(int), 0);

			SentByte = send(ClientSocket, FileNames[i], FileNameSize, 0);
			std::cout << FileNames[i] << std::endl;

			while (true)
			{
				TempFile.SendSize = fread(TempFile.Image, sizeof(char), sizeof(TempFile.Image), file);

				SentByte = send(ClientSocket, (char*)&TempFile, sizeof(TempFile), 0);

				std::cout << TempFile.SendSize << " |";

				if (feof(file))
				{
					TempFile.bIsFin = true;
					int SentByte = send(ClientSocket, (char*)&TempFile, sizeof(TempFile), 0);
					std::cout << std::endl;
					break;
				}
			}

			fclose(file);
		}
		closesocket(ClientSocket);
	}


	closesocket(ListenSocket);

	WSACleanup();

	return 0;
}