#pragma once

class Room : public std::enable_shared_from_this<Room>
{
private:
	struct PlayerGameData
	{
		PlayerRef player;

		Protocol::PlayerData data;
	};

public:
	Room();

	void Initialize();

public:
	bool Enter(PlayerRef player);
	bool Leave(PlayerRef player);
	void BroadCast(Horang::SendBufferRef sendBuffer);
	void ClientUpdate(PlayerRef player, Protocol::C_PLAY_UPDATE& pkt);

public:
	void GameStart();
	void Update();

public: // 패킷 작성
	Protocol::RoomInfo GetRoomInfo();
	void GetRoomInfo(Protocol::RoomInfo& roomInfo);
	void GetRoomInfo(Protocol::RoomInfo* roomInfo);

	void SetUpdatePacket(Protocol::S_PLAY_UPDATE& packet);
	void GetPlayerData(Protocol::PlayerData& playerData, int32 uid);

public:
	void PushJob(JobRef job);
	void FlushJob();

private:
	USE_LOCK;

	// 게임 시간
	uint64 _gameTime;

	int32 _roomID;
	std::string _roomCode;
	Protocol::eRoomState _state;

	Horang::HashMap<int32, PlayerGameData> _players;
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

class GameStartJob : public IJob
{
public:
	GameStartJob(RoomRef room, PlayerRef player)
		: _room(room), _player(player)
	{}

	virtual void Execute() override
	{
		_room->GameStart();
	}

private:
	RoomRef _room;
	PlayerRef _player;
};

class ClientUpdateJob : public IJob
{
public:
	ClientUpdateJob(RoomRef room, PlayerRef player, Protocol::C_PLAY_UPDATE pkt)
		: _room(room), _player(player), _pkt(pkt)
	{}

	virtual void Execute() override
	{
		_room->ClientUpdate(_player, _pkt);
	}

private:
	RoomRef _room;
	PlayerRef _player;
	Protocol::C_PLAY_UPDATE _pkt;
};

class UpdateJob : public IJob
{
public:
	UpdateJob(RoomRef room)
		: _room(room)
	{}

	virtual void Execute() override
	{
		_room->Update();
	}

private:
	RoomRef _room;
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