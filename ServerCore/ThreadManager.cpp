#include "pch.h"
#include "ThreadManager.h"

ThreadManager::ThreadManager()
{
	InitTLS();
}

ThreadManager::~ThreadManager()
{
	Join();
}

void ThreadManager::LaunchThread(std::function<void()> func)
{
	lock_guard<mutex> lock(_lock);

	_threads.push_back(thread([=]() 
		{
			InitTLS();
			func();
			DestroyTLS();
		}));
}

void ThreadManager::Join()
{
	for (auto& thread : _threads)
	{
		if(thread.joinable())
			thread.join();
	}
	_threads.clear();
}

void ThreadManager::InitTLS()
{
	static atomic<uint32> SThreadId = 1;
	LThreadId = SThreadId.fetch_add(1);
}

void ThreadManager::DestroyTLS()
{
}
