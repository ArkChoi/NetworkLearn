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

	SOCKET ListenSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	if (ListenSocket == INVALID_SOCKET)
	{
		std::cout << "socket Error : " << GetLastError() << std::endl;
		exit(-1);
	}

	struct sockaddr_in ListSockAddr;
	memset(&ListSockAddr, 0, sizeof(ListSockAddr));
	ListSockAddr.sin_family = PF_INET;
	ListSockAddr.sin_addr.s_addr = INADDR_ANY;
	ListSockAddr.sin_port = htons(60000);

	Result = bind(ListenSocket, (struct sockaddr*)&ListSockAddr, sizeof(ListSockAddr));
	if (Result == SOCKET_ERROR)
	{
		std::cout << "bind Error : " << GetLastError() << std::endl;
		exit(-1);
	}

	Result = listen(ListenSocket, 5);
	if (Result == SOCKET_ERROR)
	{
		std::cout << "listen Error : " << GetLastError() << std::endl;
		exit(-1);
	}

	struct sockaddr_in ClientSockAddr;
	memset(&ClientSockAddr, 0, sizeof(ClientSockAddr));
	int CluentSockAddrSize = sizeof(ClientSockAddr);

	SOCKET ClientSocket = accept(ListenSocket, (struct sockaddr*)&ClientSockAddr, &CluentSockAddrSize);

	if (ClientSocket == SOCKET_ERROR)
	{
		std::cout << "accept Error : " << GetLastError() << std::endl;
		exit(-1);
	}

	const char Message[1024] = "Hello World";
	int SentBytes = send(ClientSocket, Message, sizeof(Message), 0);
	if (SentBytes <= 0)
	{
		std::cout << "send Error : " << GetLastError() << std::endl;
		exit(-1);
	}

	char Buffer[1024] = { 0, };
	int RecvBytes = recv(ClientSocket, Buffer, sizeof(Buffer), 0);
	if (RecvBytes <= 0)
	{
		std::cout << "recv Error : " << GetLastError() << std::endl;
		exit(-1);
	}
	std::cout << Buffer << std::endl;

	closesocket(ListenSocket);

	WSACleanup();

	return 0;
}