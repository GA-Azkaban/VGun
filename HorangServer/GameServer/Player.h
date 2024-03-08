#pragma once

using RoomRef = std::shared_ptr<class Room>;

class Player
{
public:
	Player()
		: uid(0), id(""), nickname(""), ownerGameSession(nullptr), _currentRoom(nullptr)
	{}

public:
	int32 uid;
	std::string id;
	std::string nickname;

	GameSessionRef ownerGameSession;
	RoomRef _currentRoom;
};

