#include "pch.h"
#include "Session.h"
#include "SocketUtils.h"

Session::Session()
{
	_socket = SocketUtils::CreateSocket();
}

Session::~Session()
{
	SocketUtils::Close(_socket);
}

HANDLE Session::GetHandle()
{
	return reinterpret_cast<HANDLE>(_socket);
}

void Session::Dispatch(IocpEvent* iocpEvent, int32 numofBytes)
{
	switch (iocpEvent->_eventType)
	{
	case EventType::Connect:
		ProcessConnect();
		break;
	case EventType::Disconnect:

		break;
	case EventType::Send:
		ProcessSend(static_cast<SendEvent*>(iocpEvent), numofBytes);
		break;
	case EventType::Recv:
		ProcessRecv(numofBytes);
		break;
	default:

		break;
	}
}

void Session::Send(BYTE* buf, int32 len)
{
	SendEvent* sendEvent = xnew<SendEvent>();
	sendEvent->_iocpObject = shared_from_this();
	sendEvent->buffer.resize(len);
	memcpy(&sendEvent->buffer[0], buf, len);

	lock_guard<mutex> lock(_lock);
	RegisterSend(sendEvent);
}

bool Session::Connect()
{
	return RegisterConnect();
}

void Session::Disconnect()
{
}

bool Session::RegisterConnect()
{
	if (IsConnected()) return false;

	// client
	
	_connectEvent.Init();
	_connectEvent._iocpObject = shared_from_this();

	SOCKADDR_IN sockAddr;
	memset(&sockAddr, 0, sizeof(sockAddr));
	sockAddr.sin_family = AF_INET;
	sockAddr.sin_port = htons(8888);
	inet_pton(AF_INET, "127.0.0.1", &sockAddr.sin_addr);

	DWORD numofBytes = 0;
	if (SOCKET_ERROR == SocketUtils::ConnectEx(_socket, (SOCKADDR*)&sockAddr, sizeof(sockAddr), nullptr, 0, OUT & numofBytes, &_connectEvent))
	{
		int32 errCode = ::WSAGetLastError();
		if (errCode != WSA_IO_PENDING)
		{
			_connectEvent._iocpObject = nullptr;
			return false;
		}
	}

	return true;
}

bool Session::RegisterDisconnect()
{
	return false;
}

void Session::RegisterRecv()
{
	if (IsConnected() == false) return;

	_recvEvent.Init();
	_recvEvent._iocpObject = shared_from_this();

	WSABUF wsaBuf;
	wsaBuf.buf = (char*)recvBuf;
	wsaBuf.len = sizeof(recvBuf); //

	DWORD numofBytes = 0;
	DWORD flags = 0;
	if (SOCKET_ERROR == WSARecv(_socket, &wsaBuf, 1, OUT & numofBytes, OUT & flags, &_recvEvent, nullptr))
	{
		int32 errCode = ::WSAGetLastError();
		if (errCode != WSA_IO_PENDING)
		{
			_recvEvent._iocpObject = nullptr;
			//
		}
	}

}

void Session::RegisterSend(SendEvent* sendEvent)
{
	if (IsConnected() == false) return;

	WSABUF wsaBuf;
	wsaBuf.buf = (char*)&sendEvent->buffer[0];
	wsaBuf.len = (ULONG)sendEvent->buffer.size();

	DWORD numofBytes = 0;
	if (SOCKET_ERROR == WSASend(_socket, &wsaBuf, 1, OUT & numofBytes, 0, sendEvent, nullptr))
	{
		const int32 errorCode = ::WSAGetLastError();
		if (errorCode != WSA_IO_PENDING)
		{
			sendEvent->_iocpObject = nullptr;
			xdelete(sendEvent);
		}
	}

}

void Session::ProcessConnect()
{
	_connectEvent._iocpObject = nullptr;

	_isConnected.store(true);

	OnConnect();
	RegisterRecv();
}

void Session::ProcessDisconnect()
{
}

void Session::ProcessRecv(int32 numofBytes)
{
	_recvEvent._iocpObject = nullptr;

	if (numofBytes == 0)
	{
		Disconnect();
		return;
	}

	OnRecv(recvBuf, numofBytes);
	RegisterRecv();
}

void Session::ProcessSend(SendEvent* sendEvent, int32 numofBytes)
{
	sendEvent->_iocpObject = nullptr;
	xdelete(sendEvent);

	if(numofBytes == 0)
	{
		Disconnect();
		return;
	}


	OnSend(numofBytes);
}
