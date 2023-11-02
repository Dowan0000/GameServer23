#include "pch.h"

#include <iostream>
#include <WinSock2.h>
#include <WS2tcpip.h>
#pragma comment(lib, "ws2_32.lib")

int main()
{
	WSADATA wsaData;
	WSAStartup(MAKEWORD(2, 2), &wsaData);

	SOCKET clientSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	
	u_long on = 1;
	ioctlsocket(clientSocket, FIONBIO, &on);

	SOCKADDR_IN serverAddr;
	memset(&serverAddr, 0, sizeof(serverAddr));
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(8888);
	inet_pton(AF_INET, "127.0.0.1", &serverAddr.sin_addr);

	while (true)
	{
		if (connect(clientSocket, (SOCKADDR*)&serverAddr, sizeof(serverAddr)))
		{
			if (::WSAGetLastError() == WSAEWOULDBLOCK)
				continue;
			// 이미 연결된 상태라면 break
			if (::WSAGetLastError() == WSAEISCONN)
				break;
			// Error
			break;
		}
	}

	cout << "Connected to Server!" << endl;

	char sendBuffer[1024] = "Hello, Server!";
	
	while (true)
	{
		WSABUF wsaBuf;
		wsaBuf.buf = sendBuffer;
		wsaBuf.len = strlen(sendBuffer);

		DWORD sendLen = 0;
		DWORD flags = 0;
		if (WSASend(clientSocket, &wsaBuf, 1, &sendLen, flags, nullptr, nullptr))
		{
			cout << "sendData Len : " << sizeof(sendLen) << endl;
		}

		this_thread::sleep_for(1s);
	}


	closesocket(clientSocket);
	return 0;
}