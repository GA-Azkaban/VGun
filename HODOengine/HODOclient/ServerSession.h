#pragma once
#include "Session.h"
#include "Types.h"

class ServerSession : public Horang::PacketSession
{
public:
	virtual ~ServerSession();

	virtual void OnConnected() override;

	virtual void OnRecvPacket(BYTE* buffer, int32 len) override;

	virtual void OnSend(int32 len) override;

	virtual void OnDisconnected() override;
};
