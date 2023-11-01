#pragma once

class IocpEvent;


class IocpObject : public enable_shared_from_this<IocpObject>
{
public:
	virtual HANDLE	GetHandle() abstract;
	virtual void	Dispatch(IocpEvent* iocpEvent, int32 numofBytes = 0) abstract;
};


class IocpCore
{
public:
	IocpCore();
	~IocpCore();

	HANDLE	GetHandle() { return _iocpHandle; }

	bool	Register(shared_ptr<IocpObject> iocpObject);
	bool	Dispatch(uint32 timeoutMs = INFINITE);

private:
	HANDLE	_iocpHandle;
};

