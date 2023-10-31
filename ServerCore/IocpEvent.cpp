#include "pch.h"
#include "IocpEvent.h"

IocpEvent::IocpEvent(EventType type)
{
	Init();
}

void IocpEvent::Init()
{
	_overlapped.hEvent = 0;
	_overlapped.Internal = 0;
	_overlapped.InternalHigh = 0;
	_overlapped.Offset = 0;
	_overlapped.OffsetHigh = 0;
}
