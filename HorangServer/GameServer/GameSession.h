#pragma once
#include "Session.h"

class GameSession : public Horang::PacketSession
{
public:

	virtual ~GameSession() override
	{
		std::cout << "~GameSession" << std::endl;
	}

	virtual void OnConnected() override;
	virtual void OnDisconnected() override;
	virtual void OnRecvPacket(BYTE* buffer, int32 len) override;
	virtual void OnSend(int32 len) override;

public:
	void SendError(ErrorCode errorCode);

public:
	PlayerRef _player;
	std::weak_ptr<class Room> _room;
};
