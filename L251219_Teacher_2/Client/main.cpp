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

	//domain->ip  
	hostent* host = gethostbyname("yahoo.com");

	if (host)
	{
		cout << host->h_name << endl;
	}

	for (int i = 0; host->h_aliases[i]; ++i)
	{
		cout << host->h_aliases[i] << endl;
	}

	cout << (host->h_addrtype == AF_INET ? "IPv4" : "IPv6") << endl;

	IN_ADDR Address;
	//[][][][]
	for (int i = 0; i < host->h_length; ++i)
	{
		cout << inet_ntoa(*(IN_ADDR*)host->h_addr_list[i]) << endl;
	}

	//ip->domain
	char ip[1024] = "74.6.231.21";
	IN_ADDR ReverseIP;
	ReverseIP.s_addr = inet_addr(ip);

	hostent* ReverseHost = gethostbyaddr((char*)&ReverseIP, sizeof(IN_ADDR), AF_INET);
	if (ReverseHost)
	{
		cout << ReverseHost->h_name << endl;
	}


	WSACleanup();

	return 0;
}