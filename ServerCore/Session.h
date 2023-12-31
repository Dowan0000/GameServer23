#pragma once
#include "IocpCore.h"
#include "IocpEvent.h"

class Session : public IocpObject
{
	friend class Listener;

public:
	Session();
	virtual ~Session();

public:
	virtual HANDLE		GetHandle() override;
	virtual void		Dispatch(IocpEvent* iocpEvent, int32 numofBytes = 0) override;

public:
	SOCKET				GetSocket() { return _socket; }
	bool				IsConnected() { return _isConnected; }
	shared_ptr<Session> GetSession() { return static_pointer_cast<Session>(shared_from_this()); }

public:
	void				Send(BYTE* buf, int32 len);
	bool				Connect();
	void				Disconnect();

private:
	bool				RegisterConnect();
	bool				RegisterDisconnect();
	void				RegisterRecv();
	void				RegisterSend(SendEvent* sendEvent);

	void				ProcessConnect();
	void				ProcessDisconnect();
	void				ProcessRecv(int32 numofBytes);
	void				ProcessSend(SendEvent* sendEvent, int32 numofBytes);

protected:
	virtual void		OnConnect() {}
	virtual void		OnDisconnect() {}
	virtual int32		OnRecv(BYTE* buf, int32 len) { return len; }
	virtual void		OnSend(int32 len) {}

public:
	BYTE recvBuf[1024] = { 0, };

private:
	mutex			_lock;

	SOCKET			_socket;
	atomic<bool>	_isConnected = false;

private:
	ConnectEvent	_connectEvent;
	DisconnectEvent	_disconnectEvent;
	RecvEvent		_recvEvent;
};

