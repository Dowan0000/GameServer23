#include "pch.h"
#include "SocketUtils.h"

LPFN_CONNECTEX SocketUtils::ConnectEx = nullptr;
LPFN_DISCONNECTEX SocketUtils::DisconnectEx = nullptr;
LPFN_ACCEPTEX SocketUtils::AcceptEx = nullptr;

void SocketUtils::Init()
{
	WSADATA wsaData;
	WSAStartup(MAKEWORD(2, 2), OUT &wsaData);

	SOCKET socket = CreateSocket();
	BindWindowsFunc(socket, WSAID_CONNECTEX, OUT &ConnectEx);
	BindWindowsFunc(socket, WSAID_DISCONNECTEX, OUT &DisconnectEx);
	BindWindowsFunc(socket, WSAID_ACCEPTEX, OUT & AcceptEx);
}

void SocketUtils::Clear()
{
	WSACleanup();
}

bool SocketUtils::BindWindowsFunc(SOCKET socket, GUID guid, LPVOID fn)
{
	DWORD bytes = 0;
	return SOCKET_ERROR != WSAIoctl(socket, SIO_GET_EXTENSION_FUNCTION_POINTER, &guid, sizeof(guid), &fn, sizeof(fn), OUT & bytes, nullptr, nullptr);
}

SOCKET SocketUtils::CreateSocket()
{
	return WSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP, nullptr, 0, WSA_FLAG_OVERLAPPED);
}

bool SocketUtils::BindAddress(SOCKET socket, uint16 port)
{
	SOCKADDR_IN addr;
	ZeroMemory(&addr, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = htonl(INADDR_ANY);
	addr.sin_port = htons(port);
	return SOCKET_ERROR != bind(socket, (SOCKADDR*)&addr, sizeof(addr));
}

bool SocketUtils::Listen(SOCKET socket, int32 backlog)
{
	return SOCKET_ERROR != listen(socket, backlog);
}

void SocketUtils::Close(SOCKET& socket)
{
	if(socket != INVALID_SOCKET)
		closesocket(socket);
	socket = INVALID_SOCKET;
}
