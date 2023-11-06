#pragma once

class GameSession;

class GameSessionManager
{
public:
	void Add(shared_ptr<GameSession> session);
	void Remove(shared_ptr<GameSession> session);
	void BroadCast(BYTE* buf, int32 len);

private:
	mutex _lock;
	set<shared_ptr<GameSession>> _sessions;
};

extern GameSessionManager GSessionManager;