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
	
		// 

		HDData::Transform transform;
		ePlayerState state;
	};

public:
	void SetCurrentHP(int hp);
	void SetCurrentState(ePlayerState state);

	bool GetIsDie();
	int GetPlayerIndex();

private:
	// player info
	int _playerID;
	int _playerIndex;
	int _teamID;
	bool _isHost = false;
	std::string _playerNickname;

	// state info
	int _HP;
	int _kill;
	int _death;
	bool _isDie;
	ePlayerState _state;

	// weapon info
	int _weaponType = 0;
	int _bulletCount;
};

