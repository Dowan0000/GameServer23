#include "pch.h"
#include "ThreadManager.h"
#include "Memory.h"

#include "SocketUtils.h"
#include "Listener.h"


int main()
{
	/*SOCKET socket = SocketUtils::CreateSocket();
	SocketUtils::BindAddress(socket, 8888);
	SocketUtils::Listen(socket);
	SOCKET clientSocket = accept(socket, nullptr, nullptr);*/

	SOCKET socket = SocketUtils::CreateSocket();
	shared_ptr<Listener> listener = make_shared<Listener>();
	listener->StartAccept(socket, 8888);

	for (int32 i = 0; i < 5; i++)
	{
		GThreadManager->LaunchThread([=]()
			{
				while (true)
					GIocpCore->Dispatch();
			});
			
	}

	GThreadManager->Join();

	return 0;
}