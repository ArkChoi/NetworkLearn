#define _CRT_SECURE_NO_WARNINGS
#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include <iostream>
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

	ImageFile RecvImage;

	//IP v4, Steram 타입의 소켓 만들어 주셈, TCP
		//문지기
	SOCKET ServerSocket = socket(AF_INET, SOCK_STREAM, 0);

	SOCKADDR_IN ServerSockAddr;
	memset(&ServerSockAddr, 0, sizeof(ServerSockAddr));
	//ZeroMemory(&ServerSockAddr, sizeof(ServerSockAddr));
	ServerSockAddr.sin_family = PF_INET;
	ServerSockAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
	ServerSockAddr.sin_port = htons(33333); //Byte Order

	connect(ServerSocket, (SOCKADDR*)&ServerSockAddr, sizeof(ServerSockAddr));

	int FileMax = 0;
	int FileCount = 0;

	int recvByte = recv(ServerSocket, (char*)&FileMax, sizeof(int), 0);

	std::cout << FileMax << std::endl;

	while (true)
	{
		int FileNameSize = 0;
		recvByte = recv(ServerSocket, (char*)&FileNameSize, sizeof(int), 0);

		char FileName[100] = { 0, };
		recvByte = recv(ServerSocket, FileName, FileNameSize,0);

		std::cout << FileName << std::endl;
		std::cout << recvByte << std::endl;

		FILE* file = fopen(FileName, "wb");

		while (true)
		{
			recvByte = recv(ServerSocket, (char*)&RecvImage, sizeof(RecvImage), 0);

			if (recvByte <= 0)
			{
				break;
			}

			std::cout << RecvImage.bIsFin << std::endl;
			std::cout << RecvImage.SendSize << std::endl;
			std::cout << "recv" << std::endl;

			if (RecvImage.bIsFin)
			{
				std::cout << "FileCount : " << FileCount << std::endl;
				FileCount++;

				break;
			}

			fwrite(RecvImage.Image, sizeof(char), sizeof(RecvImage.Image), file);
		}

		fclose(file);

		if (FileCount == FileMax)
		{
			closesocket(ServerSocket);
			break;
		}
	}

	WSACleanup();

	return 0;
}