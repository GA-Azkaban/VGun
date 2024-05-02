#pragma once
#include "JobQueue.h"

class Room : public Horang::JobQueue, public std::enable_shared_from_this<Room>
{
	friend class RoomManager;

private:
	struct PlayerGameData
	{
		PlayerWeakRef player;

		Protocol::PlayerData data;
	};

public:
	Room();
	~Room();

	void Initialize();

public:
	bool Enter(PlayerWeakRef playerWeak);
	bool Leave(PlayerWeakRef playerWeak);
	void BroadCast(Horang::SendBufferRef sendBuffer);
	void ClientUpdate(PlayerWeakRef playerWeak, Protocol::C_PLAY_UPDATE& pkt);

public:
	void GameStart(PlayerWeakRef playerWeak);
	void Update();

public: // 패킷 작성
	Protocol::RoomInfo GetRoomInfo();
	void GetRoomInfo(Protocol::RoomInfo& roomInfo);
	void GetRoomInfo(Protocol::RoomInfo* roomInfo);
	void GetRoomInfoList(Protocol::RoomInfo* roomInfo);

	void SetUpdatePacket(Protocol::S_PLAY_UPDATE& packet);
	void GetPlayerData(Protocol::PlayerData& playerData, int32 uid);

private:
	int32 _roomId;
	std::string _roomCode;
	Protocol::eRoomState _state;

	Horang::HashMap<int32, PlayerGameData> _players;

	std::string _roomName;
	std::string _password;

	int32 _maxPlayerCount;
	int32 _currentPlayerCount;

	bool _isPrivate;
	bool _isTeam;

private:
	// 게임 시간
	uint64 _gameTime;

};

/////////////////////////////
// 
/////////////////////////////

class EnterJob : public Horang::IJob
{
public:
	EnterJob(RoomWeakRef room, PlayerWeakRef player)
		: _room(room), _player(player)
	{}

	virtual void Execute() override
	{
		_room.lock()->Enter(_player);
	}

private:
	RoomWeakRef _room;
	PlayerWeakRef _player;
};

class LeaveJob : public Horang::IJob
{
public:
	LeaveJob(RoomWeakRef room, PlayerWeakRef player)
		: _room(room), _player(player)
	{}

	virtual void Execute() override
	{
		_room.lock()->Leave(_player);
	}

private:
	RoomWeakRef _room;
	PlayerWeakRef _player;
};

class GameStartJob : public Horang::IJob
{
public:
	GameStartJob(RoomWeakRef room, PlayerWeakRef player)
		: _room(room), _player(player)
	{}

	virtual void Execute() override
	{
		_room.lock()->GameStart(_player);
	}

private:
	RoomWeakRef _room;
	PlayerWeakRef _player;
};

class ClientUpdateJob : public Horang::IJob
{
public:
	ClientUpdateJob(RoomWeakRef room, PlayerWeakRef player, Protocol::C_PLAY_UPDATE pkt)
		: _room(room), _player(player), _pkt(pkt)
	{}

	virtual void Execute() override
	{
		_room.lock()->ClientUpdate(_player, _pkt);
	}

private:
	RoomWeakRef _room;
	PlayerWeakRef _player;
	Protocol::C_PLAY_UPDATE _pkt;
};

class UpdateJob : public Horang::IJob
{
public:
	UpdateJob(RoomWeakRef room)
		: _room(room)
	{}

	virtual void Execute() override
	{
		_room.lock()->Update();
	}

private:
	RoomWeakRef _room;
};

class BroadCastJob : public Horang::IJob
{
public:
	BroadCastJob(RoomWeakRef room, Horang::SendBufferRef sendBuffer)
		: _room(room), _sendBuffer(sendBuffer)
	{}

	virtual void Execute() override
	{
		_room.lock()->BroadCast(_sendBuffer);
	}

private:
	RoomWeakRef _room;
	Horang::SendBufferRef _sendBuffer;
};