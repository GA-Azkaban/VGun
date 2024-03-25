#pragma once

class Room : public std::enable_shared_from_this<Room>
{
private:
	struct PlayerData
	{
		PlayerRef player;
		bool host = false;
		int32 team = 0;
	};

public:
	Room();

	void Initialize();

public:
	bool Enter(PlayerRef player);
	bool Leave(PlayerRef player);
	void BroadCast(Horang::SendBufferRef sendBuffer);

public:
	void Update();

public:
	Protocol::RoomInfo GetRoomInfo();
	void GetRoomInfo(Protocol::RoomInfo& roomInfo);
	void GetRoomInfo(Protocol::RoomInfo* roomInfo);

public:
	void PushJob(JobRef job);
	void FlushJob();

private:
	USE_LOCK;

	int32 _roomID;
	std::string _roomCode;
	Protocol::eRoomState _state;

	Horang::HashMap<int32, PlayerData> _players;
	JobQueue _jobs;
};

extern RoomRef GRoom;

/////////////////////////////
// 
/////////////////////////////

class EnterJob : public IJob
{
public:
	EnterJob(RoomRef room, PlayerRef player)
		: _room(room), _player(player)
	{}

	virtual void Execute() override
	{
		_room->Enter(_player);
	}

private:
	RoomRef _room;
	PlayerRef _player;
};

class LeaveJob : public IJob
{
public:
	LeaveJob(RoomRef room, PlayerRef player)
		: _room(room), _player(player)
	{}

	virtual void Execute() override
	{
		_room->Leave(_player);
	}

private:
	RoomRef _room;
	PlayerRef _player;

};

class BroadCastJob : public IJob
{
public:
	BroadCastJob(Room& room, Horang::SendBufferRef sendBuffer)
		: _room(room), _sendBuffer(sendBuffer)
	{}

	virtual void Execute() override
	{
		_room.BroadCast(_sendBuffer);
	}

private:
	Room& _room;
	Horang::SendBufferRef _sendBuffer;
};