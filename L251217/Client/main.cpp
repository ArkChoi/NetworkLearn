#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <WinSock2.h>

#pragma comment(lib, "ws2_32")

int main()
{
	WSAData wsaData;

	WSAStartup(MAKEWORD(2, 2), &wsaData);

	//IPv4, Stream 타입의 소켓, TCP 프로토콜 사용 | (AF_INET, SOCK_STREAM, 0) <- 이거 써도되는데 그 이유가 Stream 타입의 소켓에서 TCP만 남았다고.. (Windows 기준)
	//IP에 구멍 뚫고
	SOCKET SeverSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	//소켓 만들고
	SOCKADDR_IN ServerSockAddr;
	memset(&ServerSockAddr, 0, sizeof(ServerSockAddr));
	//ZeroMemory(&ServerSockAddr, sizeof(ServerSockAddr)); //위랑 같은데 Windows에서만 지정된 메크로다.

	//소켓 지정하고
	ServerSockAddr.sin_family = PF_INET;
	ServerSockAddr.sin_addr.s_addr = inet_addr("127.0.0.1"); //INADDR_ANY <- 수업용이다. , 내부 외부 상관없이 모든 값을 받는다는 것이다. 
	ServerSockAddr.sin_port = htons(777); //Byte Order 때문에 바꿔서 넣어야 한다고..

	connect(SeverSocket, (SOCKADDR*)&ServerSockAddr, sizeof(ServerSockAddr));

	char Buffer[1024] = { 0, };
	int RecvByte = recv(SeverSocket, Buffer, sizeof(Buffer), 0); //recv 와 send 는 OS 가 송수신 Buffer 에 가져오고 넣어주는 거라고 한다. | 네트웤 하는거 아님

	std::cout << Buffer << std::endl;

	char Message[1024] = "Hello Server";
	scanf("%s", &Message);

	int SentByte = send(SeverSocket, Message, (int)strlen(Message), 0); //SentByte 보내진 Byte 량 체크용

	closesocket(SeverSocket);

	WSACleanup();

	return 0;
}
