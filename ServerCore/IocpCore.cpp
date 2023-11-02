#include "pch.h"
#include "IocpCore.h"
#include "IocpEvent.h"

IocpCore::IocpCore()
{
	_iocpHandle = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, 0, 0);
}

IocpCore::~IocpCore()
{
	CloseHandle(_iocpHandle);
}

bool IocpCore::Register(shared_ptr<IocpObject> iocpObject)
{
	return CreateIoCompletionPort(iocpObject->GetHandle(), _iocpHandle, 0, 0);
}

bool IocpCore::Dispatch(uint32 timeoutMs)
{
	DWORD numberOfBytes = 0;
	ULONG_PTR key = 0;
	IocpEvent* iocpEvent = nullptr;

	bool res = GetQueuedCompletionStatus(_iocpHandle, OUT &numberOfBytes, 
		OUT &key, OUT reinterpret_cast<LPOVERLAPPED*>(&iocpEvent), timeoutMs);

	if (res)
	{
		shared_ptr<IocpObject> iocpObject = iocpEvent->_iocpObject;
		iocpObject->Dispatch(iocpEvent, numberOfBytes);
	}
	else
	{
		cout << "GetQueuedCompletionStatus failed, errCode: " << GetLastError() << endl;
		//errCode
	}


	return true;
}
