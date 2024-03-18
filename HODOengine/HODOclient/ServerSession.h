#pragma once

#include "ServerPacketHandler.h"

class ServerSession : public Horang::PacketSession
{
public:
	~ServerSession()
	{
		
	}

	virtual void OnConnected() override
	{
		
	}

	virtual void OnRecvPacket(BYTE* buffer, int32 len) override
	{
		Horang::PacketSessionRef session = GetPacketSessionRef();
		Horang::PacketHeader* header = reinterpret_cast<Horang::PacketHeader*>(buffer);

		ServerPacketHandler::HandlePacket(session, buffer, len);
	}

	virtual void OnSend(int32 len) override
	{
		
	}

	virtual void OnDisconnected() override
	{
		
	}
};
