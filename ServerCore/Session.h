#pragma once
#include "IocpCore.h"

class Session : public IocpObject
{
public:
	Session();
	virtual ~Session();

public:
	virtual HANDLE	GetHandle() override;
	virtual void	Dispatch(IocpEvent* iocpEvent, int32 numofBytes = 0) override;

public:
	SOCKET			GetSocket() { return _socket; }

public:
	char recvBuf[1024] = { 0, };

private:
	SOCKET			_socket;
	atomic<bool>	_isConnected = false;

};

