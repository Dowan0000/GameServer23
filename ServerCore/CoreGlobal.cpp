#include "pch.h"
#include "CoreGlobal.h"
#include "ThreadManager.h"
#include "IocpCore.h"
#include "SocketUtils.h"

ThreadManager* GThreadManager = nullptr;
IocpCore* GIocpCore = nullptr;

CoreGlobal::CoreGlobal()
{
	SocketUtils::Init();
	GThreadManager = new ThreadManager();
	GIocpCore = new IocpCore();
}

CoreGlobal::~CoreGlobal()
{
	SocketUtils::Clear();
	delete GThreadManager;
	delete GIocpCore;
}

CoreGlobal GCoreGlobal;