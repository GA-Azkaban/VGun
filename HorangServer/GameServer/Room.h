#pragma once

class Room : public std::enable_shared_from_this<Room>
{
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
	void PushJob(JobRef job);
	void FlushJob();

private:
	USE_LOCK;

	Horang::String _roomCode;

	Horang::HashMap<int32, PlayerRef> _players;
	JobQueue _jobs;
};

extern Room GRoom;

/////////////////////////////

class EnterJob : public IJob
{
public:
	EnterJob(Room& room, PlayerRef player)
		: _room(room), _player(player)
	{}

	virtual void Execute() override
	{
		_room.Enter(_player);
	}

private:
	Room& _room;
	PlayerRef _player;
};

class LeaveJob : public IJob
{
public:
	LeaveJob(Room& room, PlayerRef player)
		: _room(room), _player(player)
	{}

	virtual void Execute() override
	{
		_room.Leave(_player);
	}

private:
	Room& _room;
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