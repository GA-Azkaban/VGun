#pragma once
#include <vector>

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

public:
	int GetPlayerIndex();
	int GetTeamIndex();
	int GetHealth();
	int GetSpeed();
	int GetKillCount();
	int GetDeathCount();
	eWeaponType GetWeaponNow();
	std::vector<eWeaponType> GetWeaponList();
	int GetBulletCount();
	int GetCoinCount();

	void ChangeWeapon();

private:
	int _playerIndex;
	int _teamIndex;

	int _health;
	float _speed;
	//  state 나중에 추가

	int _kill;
	int _death;

	eWeaponType _weapon;
	std::vector<eWeaponType> _weaponList;
	int _bulletCount;

	int _coin;

	bool _isDie;
};

