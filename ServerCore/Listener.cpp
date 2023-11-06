#include "pch.h"
#include "Listener.h"
#include "SocketUtils.h"
#include "IocpEvent.h"
#include "Session.h"
#include "GameSession.h"

Listener::~Listener()
{
	SocketUtils::Close(_socket);
	for(auto acceptEvent : _acceptEvents)
		xdelete(acceptEvent);
}

HANDLE Listener::GetHandle()
{
	return reinterpret_cast<HANDLE>(_socket);
}

void Listener::Dispatch(IocpEvent* iocpEvent, int32 numofBytes)
{
	AcceptEvent* acceptEvent = static_cast<AcceptEvent*>(iocpEvent);
	ProcessAccept(acceptEvent);
}

bool Listener::StartAccept(uint16 port)
{
	_socket = SocketUtils::CreateSocket();
	if (_socket == INVALID_SOCKET)
		return false;

	if (false == GIocpCore->Register(shared_from_this()))
		return false;

	if (false == SocketUtils::BindAddress(_socket, port))
		return false;

	if (false == SocketUtils::Listen(_socket))
		return false;

	AcceptEvent* acceptEvent = xnew<AcceptEvent>();
	acceptEvent->_iocpObject = shared_from_this();
	_acceptEvents.push_back(acceptEvent);
	RegisterAccept(acceptEvent);

	return true;
}

void Listener::CloseSocket()
{
	SocketUtils::Close(_socket);
}

void Listener::RegisterAccept(AcceptEvent* acceptEvent)
{
	shared_ptr<GameSession> session = make_shared<GameSession>();
	GIocpCore->Register(session);

	acceptEvent->Init();
	acceptEvent->SetSession(session);

	DWORD numofBytes = 0;
	bool res = SocketUtils::AcceptEx(_socket, session->GetSocket(),
		session->recvBuf, 0, sizeof(SOCKADDR_IN) + 16, sizeof(SOCKADDR_IN) + 16,
		OUT & numofBytes, static_cast<LPOVERLAPPED>(acceptEvent));
	if (res == false)
	{
		const int32 errorCode = ::WSAGetLastError();
		if (errorCode != WSA_IO_PENDING)
		{
			// 일단 다시 Accept 걸어준다
			RegisterAccept(acceptEvent);
		}
	}
	

}

void Listener::ProcessAccept(AcceptEvent* acceptEvent)
{
	cout << "Client Connected" << endl;

	shared_ptr<GameSession> session = static_pointer_cast<GameSession>(acceptEvent->GetSession());
	if (session == nullptr)
		return;

	session->ProcessConnect();

	RegisterAccept(acceptEvent);
}
