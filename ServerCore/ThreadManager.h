#pragma once

#include <thread>
#include <functional>

class ThreadManager
{
public:
	ThreadManager();
	~ThreadManager();

	void			LaunchThread(std::function<void()> func);
	void			Join();

	static void		InitTLS();
	static void		DestroyTLS();

private:
	mutex				_lock;
	vector<thread>		_threads;
};

