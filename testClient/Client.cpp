#include "pch.h"

#include <iostream>
#include <WinSock2.h>
#include <WS2tcpip.h>
#pragma comment(lib, "ws2_32.lib")

#include "ThreadManager.h"

int main()
{
	WSAData wsaData;
	if (::WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
		return 0;

	SOCKET clientSocket = ::socket(AF_INET, SOCK_STREAM, 0);
	if (clientSocket == INVALID_SOCKET)
		return 0;

	u_long on = 1;
	if (::ioctlsocket(clientSocket, FIONBIO, &on) == INVALID_SOCKET)
		return 0;

	SOCKADDR_IN serverAddr;
	::memset(&serverAddr, 0, sizeof(serverAddr));
	serverAddr.sin_family = AF_INET;
	::inet_pton(AF_INET, "127.0.0.1", &serverAddr.sin_addr);
	serverAddr.sin_port = ::htons(8888);

	// Connect
	while (true)
	{
		if (::connect(clientSocket, (SOCKADDR*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR)
		{

			if (::WSAGetLastError() == WSAEWOULDBLOCK)
				continue;

			if (::WSAGetLastError() == WSAEISCONN)
				break;

			break;
		}
	}

	cout << "Connected to Server!" << endl;

	for (int32 i = 0; i < 5; i++)
	{
		GThreadManager->LaunchThread([=]()
			{
				while (true)
				{
					char sendBuffer[100] = "Hello World";

					int32 sendLen = send(clientSocket, sendBuffer, sizeof(sendBuffer), 0);

					cout << "Send Len : " << sendLen << endl;

					char recvBuffer[100] = { 0, };
					int32 recvLen = recv(clientSocket, recvBuffer, sizeof(recvBuffer), 0);

					cout << "Recv Len : " << recvLen << endl;

					this_thread::sleep_for(1s);
				}
			});
	}
	
	GThreadManager->Join();


	::closesocket(clientSocket);

	::WSACleanup();


	return 0;
}