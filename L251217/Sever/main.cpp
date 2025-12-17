#include <iostream>
#include <WinSock2.h>

#pragma comment(lib, "ws2_32")

int main()
{
	WSAData wsaData;

	WSAStartup(MAKEWORD(2, 2), &wsaData);

	//IPv4, Stream 타입의 소켓, TCP 프로토콜 사용 | (AF_INET, SOCK_STREAM, 0) <- 이거 써도되는데 그 이유가 Stream 타입의 소켓에서 TCP만 남았다고.. (Windows 기준)
	//IP에 구멍 뚫고
	SOCKET ListenSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	//소켓 만들고
	SOCKADDR_IN ServerSockAddr;
	memset(&ServerSockAddr, 0, sizeof(ServerSockAddr));
	//ZeroMemory(&ServerSockAddr, sizeof(ServerSockAddr)); //위랑 같은데 Windows에서만 지정된 메크로다.

	//소켓 지정하고
	ServerSockAddr.sin_family = PF_INET;
	ServerSockAddr.sin_addr.s_addr = INADDR_ANY; //INADDR_ANY <- 수업용이다. , 내부 외부 상관없이 모든 값을 받는다는 것이다. 
	ServerSockAddr.sin_port = htons(777); //Byte Order 때문에 바꿔서 넣어야 한다고..

	//IP 와 소켓, App 을 연결한다
	bind(ListenSocket, (SOCKADDR*)&ServerSockAddr, sizeof(ServerSockAddr));

	listen(ListenSocket, 5);

	SOCKADDR_IN ClientSockAddr;
	memset(&ClientSockAddr, 0, sizeof(ClientSockAddr));
	int ClientSockAddrSize = sizeof(ClientSockAddr);

	//WebSever 기본
	//Stateless Server
	while (true)
	{
		// Return 값이 없으면 대기에 걸린다, blocking socket 이라고 함
		SOCKET ClientSocket = accept(ListenSocket, (SOCKADDR*)&ClientSockAddr, &ClientSockAddrSize);


		char Message[] = "Hello Client";
		int SentByte = send(ClientSocket, Message, (int)strlen(Message), 0); //SentByte 보내진 Byte 량 체크용

		char Buffer[1024] = { 0, };
		int RecvByte = recv(ClientSocket, Buffer, sizeof(Buffer), 0); //recv 와 send 는 OS 가 송수신 Buffer 에 가져오고 넣어주는 거라고 한다. | 네트웤 하는거 아님

		std::cout << Buffer << std::endl;

		closesocket(ClientSocket);
	}

	//GameServer
	//Statefull Server
	//만들기 어렵다는 단점이 존재 -> 비싸진다.

	closesocket(ListenSocket);

	WSACleanup();

	return 0;
}