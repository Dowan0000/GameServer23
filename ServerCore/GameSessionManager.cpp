#include "pch.h"
#include "GameSessionManager.h"
#include "GameSession.h"

GameSessionManager GSessionManager;

void GameSessionManager::Add(shared_ptr<GameSession> session)
{
	lock_guard<mutex> lock(_lock);
	_sessions.insert(session);
}

void GameSessionManager::Remove(shared_ptr<GameSession> session)
{
	lock_guard<mutex> lock(_lock);
	_sessions.erase(session);
}

void GameSessionManager::BroadCast(BYTE* buf, int32 len)
{
	lock_guard<mutex> lock(_lock);
	for (auto& session : _sessions)
	{
		session->Send(buf, len);
	}
}
