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
	return HANDLE();
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
	Session* session = xnew<Session>();

	acceptEvent->Init();
	acceptEvent->SetSession(session);

	DWORD recvBytes = 0;
	bool res = SocketUtils::AcceptEx(_socket, session->GetSocket(), 
		session->recvBuf, 0, sizeof(SOCKADDR_IN) + 16, sizeof(SOCKADDR_IN) + 16, 
		OUT & recvBytes, static_cast<LPOVERLAPPED>(acceptEvent));
	if (res == false)
	{
		// errCode
		RegisterAccept(acceptEvent);
	}
	

}

void Listener::ProcessAccept(AcceptEvent* acceptEvent)
{
	// process
}
