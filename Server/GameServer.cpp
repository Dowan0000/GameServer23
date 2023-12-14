#include "pch.h"
#include "ThreadManager.h"
#include "Memory.h"

#include "SocketUtils.h"
#include "Listener.h"
#include "Session.h"


int main()
{
	shared_ptr<Listener> listener = make_shared<Listener>();
	listener->StartAccept(8765);

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