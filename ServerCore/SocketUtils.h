#pragma once

class SocketUtils
{
public:
	static LPFN_CONNECTEX	    ConnectEx;
	static LPFN_DISCONNECTEX	DisconnectEx;
	static LPFN_ACCEPTEX		AcceptEx;

public:
	static void Init();
	static void Clear();

	static bool BindWindowsFunc(SOCKET socket, GUID guid, LPVOID* fn);
	static SOCKET CreateSocket();

	static bool BindAddress(SOCKET socket, uint16 port);
	static bool Listen(SOCKET socket, int32 backlog = SOMAXCONN);
	static void Close(SOCKET& socket);

};

