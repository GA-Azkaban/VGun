#pragma once
#include "JobQueue.h"

class Room : public Horang::JobQueue
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

	void Initialize(int32 roomId = 0, std::string roomCode = "0000");

public:
	bool Enter(PlayerWeakRef playerWeak, std::string password = "");
	bool Leave(PlayerWeakRef playerWeak, int32 uid);
	void BroadCast(Horang::SendBufferRef sendBuffer);
	void ClientUpdate(PlayerWeakRef playerWeak, Protocol::C_PLAY_UPDATE& pkt);

public:
	void RoomStart(PlayerWeakRef playerWeak);
	void GameStart();
	void Update();
	void GameEnd(bool forceEnd = false, uint64 startTime = 0);

	void GameEndToLobby();

public: // 방 기능
	// 강퇴
	void Kick(PlayerWeakRef playerWeak, std::string targetNickName = "");

	// 팀 변경
	void ChangeTeam(PlayerWeakRef playerWeak, Protocol::eTeamColor color, std::string targetNickName = "");

	void RoomChat(PlayerWeakRef playerWeak, std::string chat);

public: // 인게임 플레이어 관련
	void PlayJump(PlayerWeakRef playerWeak);
	void PlayShoot(PlayerWeakRef shooterWeak, uint64 targetUid, Protocol::eHitLocation hitLocation);
	void PlayHit(uint64 hitPlayerUid, Protocol::eHitLocation hitLocation);
	void PlayKill(uint64 killPlayerUid, uint64 deathPlayerUid);
	void PlayRespawn(uint64 deathPlayerUid);
	void PlayRoll(uint64 playerUid);
	void PlayReload(uint64 playerUid);

public: // 패킷 작성
	Protocol::RoomInfo GetRoomInfo();
	void GetRoomInfo(Protocol::RoomInfo& roomInfo);
	void GetRoomInfo(Protocol::RoomInfo* roomInfo);
	void GetRoomInfoList(Protocol::RoomInfo* roomInfo);

	void SetUpdatePacket(Protocol::S_PLAY_UPDATE& packet);
	void GetPlayerData(Protocol::PlayerData* playerData, int32 uid);

public:
	RoomRef GetSharedRef() { return this->_roomRef; }
	RoomWeakRef GetWeakRef() { return this->_roomWeakRef; }

private:
	int32 _roomId;
	std::string _roomCode;
	Protocol::eRoomState _state;

	Horang::HashMap<int32, PlayerGameData> _players;

	std::string _roomName;
	std::string _password;

	int32 _maxPlayerCount;

	bool _isPrivate;
	bool _isTeam;

	Protocol::GameRule _gameRule;

private:
	// 게임 시간
	uint64 _gameStartTime;

	// Todo Debug 
	uint64 _lastUpdateTime;

	RoomRef _roomRef;
	RoomWeakRef _roomWeakRef;
};

/////////////////////////////
// 
/////////////////////////////

class EnterJob : public Horang::IJob
{
public:
	EnterJob(RoomWeakRef room, PlayerWeakRef player, std::string password = "")
		: _room(room), _player(player), _password(password)
	{}

	virtual void Execute() override
	{
		_room.lock()->Enter(_player, _password);
	}

private:
	RoomWeakRef _room;
	PlayerWeakRef _player;
	std::string _password;
};

class LeaveJob : public Horang::IJob
{
public:
	LeaveJob(RoomWeakRef room, PlayerWeakRef player, int32 uid)
		: _room(room), _player(player), uid(uid)
	{}

	virtual void Execute() override
	{
		_room.lock()->Leave(_player, uid);
	}

private:
	RoomWeakRef _room;
	PlayerWeakRef _player;
	int32 uid;
};

class KickJob : public Horang::IJob
{
public:
	KickJob(RoomWeakRef room, PlayerWeakRef player, std::string targetNickName)
		: _room(room), _player(player), _targetNickName(targetNickName)
	{}

	virtual void Execute() override
	{
		_room.lock()->Kick(_player, _targetNickName);
	}

private:
	RoomWeakRef _room;
	PlayerWeakRef _player;
	std::string _targetNickName;
};

class TeamChangeJob : public Horang::IJob
{
public:
	TeamChangeJob(RoomWeakRef room, PlayerWeakRef player, Protocol::eTeamColor color, std::string targetNickName)
		: _room(room), _player(player), _color(color), _targetNickName(targetNickName)
	{}

	virtual void Execute() override
	{
		_room.lock()->ChangeTeam(_player, _color, _targetNickName);
	}

private:
	RoomWeakRef _room;
	PlayerWeakRef _player;
	Protocol::eTeamColor _color;
	std::string _targetNickName;
};

class RoomStartJob : public Horang::IJob
{
public:
	RoomStartJob(RoomWeakRef room, PlayerWeakRef player)
		: _room(room), _player(player)
	{}

	virtual void Execute() override
	{
		_room.lock()->RoomStart(_player);
	}

private:
	RoomWeakRef _room;
	PlayerWeakRef _player;
};

class GameStartJob : public Horang::IJob
{
public:
	GameStartJob(RoomWeakRef room)
		: _room(room)
	{}

	virtual void Execute() override
	{
		_room.lock()->GameStart();
	}

private:
	RoomWeakRef _room;
};

class GameEndJob : public Horang::IJob
{
public:
	GameEndJob(RoomWeakRef room, bool forceEnd = false, uint64 startTime = 0)
		: _room(room), _forceEnd(forceEnd), _startTime(startTime)
	{}

	virtual void Execute() override
	{
		_room.lock()->GameEnd(_forceEnd, _startTime);
	}

private:
	RoomWeakRef _room;
	bool _forceEnd;
	uint64 _startTime;
};

class GameEndToLobbyJob : public Horang::IJob
{
public:
	GameEndToLobbyJob(RoomWeakRef room)
		: _room(room)
	{}

	virtual void Execute() override
	{
		_room.lock()->GameEndToLobby();
	}

private:
	RoomWeakRef _room;
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

class PlayJumpJob : public Horang::IJob
{
public:
	PlayJumpJob(RoomWeakRef room, PlayerWeakRef player)
		: _room(room), _player(player)
	{}

	virtual void Execute() override
	{
		_room.lock()->PlayJump(_player);
	}

private:
	RoomWeakRef _room;
	PlayerWeakRef _player;
};

class PlayShootJob : public Horang::IJob
{
public:
	PlayShootJob(RoomWeakRef room, PlayerWeakRef shooter, uint64 targetUid, Protocol::eHitLocation hitLocation)
		: _room(room), _shooter(shooter), _targetUid(targetUid), _hitLocation(hitLocation)
	{}

	virtual void Execute() override
	{
		_room.lock()->PlayShoot(_shooter, _targetUid, _hitLocation);
	}

private:
	RoomWeakRef _room;
	PlayerWeakRef _shooter;
	uint64 _targetUid;
	Protocol::eHitLocation _hitLocation;
};

class PlayRespawnJob : public Horang::IJob
{
public:
	PlayRespawnJob(RoomWeakRef room, uint64 deathPlayerUid)
		: _room(room), _deathPlayerUid(deathPlayerUid)
	{}

	virtual void Execute() override
	{
		_room.lock()->PlayRespawn(_deathPlayerUid);
	}

private:
	RoomWeakRef _room;
	uint64 _deathPlayerUid;
};

class PlayRollJob : public Horang::IJob
{
public:
	PlayRollJob(RoomWeakRef room, uint64 playerUid)
		: _room(room), _playerUid(playerUid)
	{}

	virtual void Execute() override
	{
		_room.lock()->PlayRoll(_playerUid);
	}

private:
	RoomWeakRef _room;
	uint64 _playerUid;
};

class PlayReloadJob : public Horang::IJob
{
public:
	PlayReloadJob(RoomWeakRef room, uint64 playerUid)
		: _room(room), _playerUid(playerUid)
	{}

	virtual void Execute() override
	{
		_room.lock()->PlayReload(_playerUid);
	}

private:
	RoomWeakRef _room;
	uint64 _playerUid;
};

class RoomChatJob : public Horang::IJob
{
public:
	RoomChatJob(RoomWeakRef room, PlayerWeakRef player, std::string chat)
		: _room(room), _player(player), _chat(chat)
	{}

	virtual void Execute() override
	{
		_room.lock()->RoomChat(_player, _chat);
	}

private:
	RoomWeakRef _room;
	PlayerWeakRef _player;
	std::string _chat;
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