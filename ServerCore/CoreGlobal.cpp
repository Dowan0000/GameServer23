#include "pch.h"
#include "CoreGlobal.h"
#include "ThreadManager.h"
#include "IocpCore.h"
#include "SocketUtils.h"

ThreadManager* GThreadManager = nullptr;

CoreGlobal::CoreGlobal()
{
	SocketUtils::Init();
	GThreadManager = new ThreadManager();
}

CoreGlobal::~CoreGlobal()
{
	SocketUtils::Clear();
	delete GThreadManager;
}

CoreGlobal GCoreGlobal;