#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <WinSock2.h>
#include <iostream>

#pragma comment (lib, "ws2_32")

int main()
{
	WSADATA WsaData;
	int Result = WSAStartup(MAKEWORD(2, 2), &WsaData);
	if (Result < 0)
	{
		std::cout << "Error : " << GetLastError() << std::endl;
		exit(-1);
	}

	SOCKET SeverSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	if (SeverSocket == INVALID_SOCKET)
	{
		std::cout << "socket Error : " << GetLastError() << std::endl;
		exit(-1);
	}

	struct sockaddr_in ServerSocketAddr;
	memset(&ServerSocketAddr, 0, sizeof(ServerSocketAddr));
	ServerSocketAddr.sin_family = PF_INET;
	ServerSocketAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
	ServerSocketAddr.sin_port = htons(60000);

	Result = connect(SeverSocket, (struct sockaddr*)&ServerSocketAddr, sizeof(ServerSocketAddr));
	if (Result == SOCKET_ERROR)
	{
		std::cout << "connect Error : " << GetLastError() << std::endl;
		exit(-1);
	}

	/*Result = listen(SeverSocket, 5);
	if (Result == SOCKET_ERROR)
	{
		std::cout << "listen Error : " << GetLastError() << std::endl;
		exit(-1);
	}

	struct sockaddr_in ClientSockAddr;
	memset(&ClientSockAddr, 0, sizeof(ClientSockAddr));
	int CluentSockAddrSize = sizeof(ClientSockAddr);

	SOCKET ClientSocket = accept(SeverSocket, (struct sockaddr*)&ClientSockAddr, &CluentSockAddrSize);

	if (ClientSocket == SOCKET_ERROR)
	{
		std::cout << "accept Error : " << GetLastError() << std::endl;
		exit(-1);
	}*/

	char Buffer[1024] = { 0, };
	int RecvBytes = recv(SeverSocket, Buffer, sizeof(Buffer), 0);
	if (RecvBytes <= 0)
	{
		std::cout << "recv Error : " << GetLastError() << std::endl;
		exit(-1);
	}
	std::cout << Buffer << std::endl;

	const char Message[1024] = "Hello Sever";
	int SentBytes = send(SeverSocket, Message, sizeof(Message), 0);
	if (SentBytes <= 0)
	{
		std::cout << "send Error : " << GetLastError() << std::endl;
		exit(-1);
	}

	closesocket(SeverSocket);

	WSACleanup();

	return 0;
}