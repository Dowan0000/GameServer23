#include "pch.h"
#include "ThreadManager.h"
#include "Memory.h"

#include "SocketUtils.h"
#include "Listener.h"

int main()
{
	/*SOCKET socket = SocketUtils::CreateSocket();
	SocketUtils::BindAddress(socket, 7777);
	SocketUtils::Listen(socket);
	SOCKET clientSocket = accept(socket, nullptr, nullptr);*/

	SOCKET socket = SocketUtils::CreateSocket();
	Listener listener;
	listener.StartAccept(socket, 8888);

	for (int32 i = 0; i < 5; i++)
	{
		GThreadManager->LaunchThread([=]()
			{
				while (true)
					GIocpCore->Dispatch();
			});
			
	}


	return 0;
}