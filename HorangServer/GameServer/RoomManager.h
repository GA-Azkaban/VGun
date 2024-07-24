#pragma once
#include "Room.h"

class RoomManager : public Horang::JobQueue
{
public:
	RoomManager();
	~RoomManager();

public:
	void Initialize();

	RoomRef CreateRoom(std::string roomName = "DefaultRoomName", std::string password = "", int32 maxPlayerCount = 6, bool isPrivate = false, bool isTeam = false, int32 dummyClient = 0);

	void DestroyRoom(RoomRef room);

public:
	void RoomListUpdate();
	void SendRoomList(PlayerWeakRef player);

public:
	void EnterRoom(PlayerWeakRef player, int32 roomId, std::string password = "");

private:
	Horang::HashMap<int32, RoomRef> _rooms;

	Horang::Vector<RoomRef> _roomPool;

	Protocol::S_ROOM_LIST _roomList;
};

using RoomManagerRef = std::shared_ptr<RoomManager>;
extern RoomManagerRef GRoomManager;

class CreateRoomJob : public Horang::IJob
{
public:
	CreateRoomJob(PlayerWeakRef player, std::string roomName, std::string password, int32 maxPlayerCount, bool isPrivate, bool isTeam)
		: _player(player), _roomName(roomName), _password(password), _maxPlayerCount(maxPlayerCount), _isPrivate(isPrivate), _isTeam(isTeam)
	{}

	virtual void Execute() override
	{
		RoomRef room = GRoomManager->CreateRoom(_roomName, _password, _maxPlayerCount, _isPrivate, _isTeam);

		room->Push(Horang::MakeShared<EnterJob>(room->GetWeakRef(), _player, _password));
	}

private:
	PlayerWeakRef _player;
	std::string _roomName;
	std::string _password;
	int32 _maxPlayerCount;
	bool _isPrivate;
	bool _isTeam;
};

class DestroyRoomJob : public Horang::IJob
{
public:
	DestroyRoomJob(RoomWeakRef room)
		: _room(room)
	{}

	virtual void Execute() override
	{
		GRoomManager->DestroyRoom(_room.lock());
	}

private:
	RoomWeakRef _room;
};

class RoomListUpdateJob : public Horang::IJob
{
public:
	virtual void Execute() override
	{
		GRoomManager->RoomListUpdate();
	}
};

class SendRoomListJob : public Horang::IJob
{
public:
	SendRoomListJob(PlayerWeakRef player)
		: _player(player)
	{}

	virtual void Execute() override
	{
		GRoomManager->SendRoomList(_player);
	}

private:
	PlayerWeakRef _player;
};

class EnterRoomJob : public Horang::IJob
{
public:
	EnterRoomJob(PlayerWeakRef player, int32 roomId, std::string password = "")
		: _player(player), _roomId(roomId), _password(password)
	{}

	virtual void Execute() override
	{
		GRoomManager->EnterRoom(_player, _roomId, _password);
	}

private:
	PlayerWeakRef _player;
	int32 _roomId;
	std::string _password;
};