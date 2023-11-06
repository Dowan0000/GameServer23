#include "pch.h"
#include "GameSession.h"
#include "GameSessionManager.h"

void GameSession::OnConnect()
{
	GSessionManager.Add(static_pointer_cast<GameSession>(shared_from_this()));
}

void GameSession::OnDisconnect()
{
	GSessionManager.Remove(static_pointer_cast<GameSession>(shared_from_this()));
}

int32 GameSession::OnRecv(BYTE* buf, int32 len)
{
	cout << "GameSession::OnRecv" << endl;
	cout << "recv Len : " << len << endl;

	GSessionManager.BroadCast(buf, len);

	return len;
}

void GameSession::OnSend(int32 len)
{
	cout << "GameSession::OnSend" << endl;
}
