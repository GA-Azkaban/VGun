#pragma once
#include "../HODOengine/HODO_API.h"
#include "Weapon.h"

/// <summary>
/// 플레이어 정보 - 오수안
/// [가지고 있어야 하는 정보]
/// index, 킬 수, 다이 수, 팀, 체력, 현재 총 정보, 현재 총알 수, 코인 수, 속도, 현재 상태, 
/// </summary>

class Player : public HDData::Script
{
public:
	void Start() override;
	void Update() override;
	void LateUpdate() override;

private:
	int _playerIndex;
	int _teamIndex;

	int _health;
	float _speed;
	// state 나중에 추가

	int _kill;
	int _death;

	eWeaponType _weapon;
	int _bulletCount;

	int _coin;
};

