#pragma once
#include "../HODOengine/HODO_API.h"
#include "PlayerState.h"

enum class eHITLOC
{
	NONE = 0,
	NO_HIT = 1,
	HEAD = 2,
	BODY = 3
};

class PlayerInfo : public HDData::Script
{
public:
	PlayerInfo();
	PlayerInfo(PlayerInfo* info);

public:
	void Start() override;
	void Update() override;

public:
	void Init();

public:
	void SetPlayerUID(int uid);
	void SetPlayerID(std::string id);
	void PlayerIndex(int index);
	void SetTeamID(eTeam team);
	void SetIsHost(bool isHost);
	void SetNickName(std::string nickName);

	void SetCurrentHP(int hp);
	void SetCurrentKill(int killCount);
	void SetCurrentDeath(int deathCount);
	void SetIsDie(bool isDie);
	void SetCurrentState(ePlayerState state);

	void SetCurrentBulletCount(int Count);

	int& GetPlayerUID();
	std::string& GetPlayerID();
	bool& GetIsHost();
	int& GetPlayerIndex();
	eTeam& GetPlayerTeam();
	bool& GetPlayerDie();
	std::string& GetPlayerNickName();

	int& GetPlayerCurrentHP();
	int& GetPlayerKillCount();
	int& GetPlayerDeathCount();
	int& GetCurrentBulletCount();
	bool& GetIsDie();
	ePlayerState& GetPlayerState();

	void SetIsMyInfo(bool isMine);
	bool GetIsMyInfo();

	
	void OtherPlayerShoot(eHITLOC loc);
private:
	bool _isMyInfo = false;

	// player info
	int _playerUID;
	std::string _playerID;
	int _playerIndex;
	eTeam _teamID;
	bool _isHost = false;
	std::string _playerNickname;

	// state info
	bool _isJump;
	bool _isCrouch;
	bool _isShoot;

	int _currentHP;
	bool _isDie;

	int _bulletCount = 30;

	ePlayerState _state;
	
	// count info
	int _kill;
	int _death;
};

