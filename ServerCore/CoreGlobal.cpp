#include "pch.h"
#include "CoreGlobal.h"
#include "ThreadManager.h"
#include "IocpCore.h"

ThreadManager* GThreadManager = nullptr;
IocpCore* GIocpCore = nullptr;

CoreGlobal::CoreGlobal()
{
	GThreadManager = new ThreadManager();
	GIocpCore = new IocpCore();
}

CoreGlobal::~CoreGlobal()
{
	delete GThreadManager;
	delete GIocpCore;
}

CoreGlobal GCoreGlobal;