#pragma once
#include <vector>

#include "../HODOengine/HODO_API.h"
#include "Weapon.h"

/// <summary>
/// 플레이어 정보 - 오수안
/// [가지고 있어야 하는 정보]
/// index, 킬 수, 다이 수, 팀, 체력, 현재 총 정보, 현재 총알 수, 속도, 현재 상태, 
/// 상점은 죽어야만 열린다
/// </summary>

enum class eUpperState
{
	IDLE,
	RUN,
	SHOOT,
	CROUCH
};

enum class eLowerState
{
	IDLE,
	RUN,
	SHOOT,
	CROUCH
};

class Player : public HDData::Script
{
public:
	void Start() override;
	void Update() override;
	void LateUpdate() override;

public:
	int GetPlayerIndex();
	int GetTeamIndex();
	int GetHealth();
	int GetSpeed();
	int GetKillCount();
	int GetDeathCount();
	eWeaponType GetWeaponNow();
	int GetBulletCount();

	void ChangeWeapon(eWeaponType type);

private:
	int _playerIndex;
	std::string _playerName;
	int _characterType;
	int _teamID;

	int _health;
	float _speed;
	//  state 나중에 추가

	int _kill;
	int _death;

	eWeaponType _weapon;
	
	int _bulletMax;
	int _bulletCount;
	int _dealBody;
	int _dealHead;
	float _reloadTime;
	float _shootTime;

	bool _isDie;

	eUpperState _upperState;
	eLowerState _lowerState;
};

