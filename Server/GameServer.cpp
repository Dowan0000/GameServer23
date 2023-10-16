#include "pch.h"
#include "ThreadManager.h"

#include "winsock2.h"

#pragma comment(lib, "ws2_32.lib")

int main()
{
	GThreadManager->LaunchThread([]()
		{
			cout << "12345" << endl;
		});




	/*WSAData wsaData;
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
	{
		cout << "WSAStartup() error!" << endl;
		exit(-1);
	}

	SOCKET ListenSocket = socket(AF_INET, SOCK_STREAM, 0);
	if (ListenSocket == INVALID_SOCKET)
	{
		cout << "socket() error!" << endl;
		exit(-1);
	}

	SOCKADDR_IN ServerAddr;
	ZeroMemory(&ServerAddr, sizeof(ServerAddr));
	ServerAddr.sin_family = AF_INET;
	ServerAddr.sin_port = htons(8765);
	ServerAddr.sin_addr.s_addr = htonl(INADDR_ANY);

	if (bind(ListenSocket, (SOCKADDR*)&ServerAddr, sizeof(ServerAddr)) == SOCKET_ERROR)
	{
		cout << "bind() error!" << endl;
		exit(-1);
	}

	if (listen(ListenSocket, SOMAXCONN) == SOCKET_ERROR)
	{
		cout << "listen() error!" << endl;
		exit(-1);
	}

	SOCKADDR_IN ClientAddr;
	ZeroMemory(&ClientAddr, sizeof(ClientAddr));
	int AddrLen = sizeof(ClientAddr);
	SOCKET ClientSocket = accept(ListenSocket, (SOCKADDR*)&ClientAddr, &AddrLen);
	if (ClientSocket == INVALID_SOCKET)
	{
		cout << "accept() error!" << endl;
		exit(-1);
	}

	WSACleanup();*/




	return 0;
}