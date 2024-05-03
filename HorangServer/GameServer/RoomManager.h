#pragma once
#include "Room.h"

class RoomManager : public Horang::JobQueue
{
public:
	RoomManager();
	~RoomManager();

public:
	RoomRef CreateRoom(std::string roomName = "DefaultRoomName", std::string password = "", int32 maxPlayerCount = 6, bool isPrivate = false, bool isTeam = false);

	void DestroyRoom(RoomRef room);

public:
	void RoomListUpdate();
	void SendRoomList(PlayerWeakRef player);

public:
	void EnterRoom(PlayerWeakRef player, int32 roomId);

private:
	Horang::HashMap<int32, RoomRef> _rooms;

	Horang::Vector<RoomRef> _roomPool;

	Protocol::S_ROOM_LIST _roomList;
};

extern RoomManager GRoomManager;

class CreateRoomJob : public Horang::IJob
{
public:
	CreateRoomJob(PlayerWeakRef player, std::string roomName, std::string password, int32 maxPlayerCount, bool isPrivate, bool isTeam)
		: _player(player), _roomName(roomName), _password(password), _maxPlayerCount(maxPlayerCount), _isPrivate(isPrivate), _isTeam(isTeam)
	{}

	virtual void Execute() override
	{
		auto room = GRoomManager.CreateRoom(_roomName, _password, _maxPlayerCount, _isPrivate, _isTeam);

		if (room)
			room->Push(Horang::MakeShared<EnterJob>(room, _player));
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
	DestroyRoomJob(RoomRef room)
		: _room(room)
	{}

	virtual void Execute() override
	{
		GRoomManager.DestroyRoom(_room);
	}

private:
	RoomRef _room;
};

class RoomListUpdateJob : public Horang::IJob
{
public:
	virtual void Execute() override
	{
		GRoomManager.RoomListUpdate();
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
		GRoomManager.SendRoomList(_player);
	}

private:
	PlayerWeakRef _player;
};

class EnterRoomJob : public Horang::IJob
{
public:
	EnterRoomJob(PlayerWeakRef player, int32 roomId)
		: _player(player), _roomId(roomId)
	{}

	virtual void Execute() override
	{
		GRoomManager.EnterRoom(_player, _roomId);
	}

private:
	PlayerWeakRef _player;
	int32 _roomId;
};