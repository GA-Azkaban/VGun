#pragma once

class Player
{
public:
	Player()
		: uid(0), id(""), nickname(""), ownerGameSession(nullptr), _currentRoom(nullptr)
	{}

public:
	void GetUserInfo(Protocol::UserInfo* userInfo);
	void GetUserInfo(Protocol::UserInfo& userInfo);

public:
	int32 uid;
	std::string id;
	std::string nickname;

	GameSessionRef ownerGameSession;
	RoomRef _currentRoom;
};

