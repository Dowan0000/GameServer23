#include "pch.h"
#include "Listener.h"
#include "SocketUtils.h"
#include "IocpEvent.h"
#include "Session.h"

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

bool Listener::StartAccept(SOCKET listenSocket, uint16 port)
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
	shared_ptr<Session> session = make_shared<Session>();

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
	// process

	cout << "Client Connected" << endl;
}
