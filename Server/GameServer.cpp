#include "pch.h"
#include "ThreadManager.h"
#include "Memory.h"

#include "SocketUtils.h"
#include "Listener.h"
#include "Session.h"

class GameSession : public Session
{
public:
	virtual int32 OnRecv(BYTE* buf, int32 len) override
	{
		cout << "OnRecv Len = " << len << endl;
		Send(buf, len);
		return len;
	}

	virtual void OnSend(int32 len) override
	{
		cout << "OnSend Len = " << len << endl;
	}
};

int main()
{
	/*SOCKET socket = SocketUtils::CreateSocket();
	SocketUtils::BindAddress(socket, 8888);
	SocketUtils::Listen(socket);
	SOCKET clientSocket = accept(socket, nullptr, nullptr);*/

	shared_ptr<Listener> listener = make_shared<Listener>();
	listener->StartAccept(8888);

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