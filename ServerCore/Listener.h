#pragma once
#include "IocpCore.h"

class AcceptEvent;

class Listener : public IocpObject
{
public:
	Listener() {}
	~Listener();

public:
	virtual HANDLE	GetHandle() override;
	virtual void	Dispatch(IocpEvent* iocpEvent, int32 numofBytes = 0) override;

public:
	bool StartAccept(uint16 port);
	void CloseSocket();

private:
	void RegisterAccept(AcceptEvent* acceptEvent);
	void ProcessAccept(AcceptEvent* acceptEvent);

private:
	SOCKET					_socket = INVALID_SOCKET;
	vector<AcceptEvent*>	_acceptEvents;
};

