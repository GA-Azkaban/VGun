#pragma once

using RoomRef = std::shared_ptr<class Room>;

class RoomManager
{
public:
	RoomManager();

	void Push(RoomRef room);
	RoomRef Pop();

	RoomRef CreateRoom();
	RoomRef GetRoom(Horang::String roomCode);

	void LeaveRoom(PlayerRef player);

	void Update();

private:
	USE_LOCK;
	Horang::HashMap<Horang::String, RoomRef> _rooms;
	Horang::Vector<RoomRef> _roomPool;
};

extern RoomManager GRoomManager;