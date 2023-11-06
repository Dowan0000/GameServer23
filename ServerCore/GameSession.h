#pragma once

#include "Session.h"

class GameSession : public Session
{
public:
	virtual void		OnConnect() override;
	virtual void		OnDisconnect() override;
	virtual int32		OnRecv(BYTE* buf, int32 len) override;
	virtual void		OnSend(int32 len) override;
};