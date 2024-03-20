#pragma once

using RoomRef = std::shared_ptr<class Room>;

class RoomManager
{
public:
	RoomManager();

	RoomRef GetRoom(std::string roomCode);

	void LeaveRoom(PlayerRef player);

public:
	void PushJob(JobRef job);
	void FlushJob();

private:
	USE_LOCK;
	Horang::HashMap<std::string, RoomRef> _rooms;
	Horang::Vector<RoomRef> _roomPool;
};

extern RoomManager GRoomManager;


