#pragma once
#include "../HODOengine/HODO_API.h"
#include "PlayerState.h"

class PlayerInfo : public HDData::Script
{
public:
	PlayerInfo();

public:
	void Start() override;
	void Update() override;

public:
	void Init();

	struct Stats 
	{
		// from server to client

		int playerIndex;

		int health;

		bool damageHead;
		bool damageBody;
		bool damageArmlegs;

		int bulletCount;

		HDData::Transform transform;
		ePlayerState state;
	};

public:
	void SetPlayerID(int id);
	void PlayerIndex(int index);
	void SetTeamID(eTeam team);
	void SetIsHost(bool isHost);
	void SetNickName(std::string nickName);

	void SetMaxHP(int hp);
	void SetCurrentHP(int hp);
	void SetCurrentKill(int killCount);
	void SetCurrentDeath(int deathCount);
	void SetIsDie(bool isDie);
	void SetCurrentState(ePlayerState state);

	void SetCurrentBulletCount(int Count);

	int GetPlayerID();
	bool GetIsHost();
	int GetPlayerIndex();
	eTeam GetPlayerTeam();
	bool GetPlayerDie();
	std::string GetPlayerNickName();

	int GetPlayerMaxHP();
	int GetPlayerCurrentHP();
	int GetPlayerKillCount();
	int GetPlayerDeathCount();
	bool GetIsDie();
	ePlayerState GetPlayerState();


private:
	// player info
	int _playerID;
	int _playerIndex;
	eTeam _teamID;
	bool _isHost = false;
	std::string _playerNickname;

	// state info
	int _maxHP;
	int _currentHP;
	int _kill;
	int _death;
	bool _isDie;
	ePlayerState _state;

	// weapon info
	int _weaponType = 0;
	int _bulletCount;
};

