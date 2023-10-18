#include "pch.h"
#include "ThreadManager.h"
#include "Memory.h"

#include "SocketUtils.h"

int main()
{
	SOCKET socket = SocketUtils::CreateSocket();
	SocketUtils::BindAddress(socket, 7777);
	SocketUtils::Listen(socket);
	SOCKET clientSocket = accept(socket, nullptr, nullptr);


	



	return 0;
}