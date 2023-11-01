#include "pch.h"
#include "Listener.h"
#include "SocketUtils.h"
#include "IocpEvent.h"

Listener::~Listener()
{

}

HANDLE Listener::GetHandle()
{
	return HANDLE();
}

void Listener::Dispatch(IocpEvent* iocpEvent, int32 numofBytes)
{
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
}

void Listener::RegisterAccept(AcceptEvent* acceptEvent)
{

	// SocketUtils::AcceptEx


}

void Listener::ProcessAccept(AcceptEvent* acceptEvent)
{
}
