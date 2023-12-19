#pragma once
#include "../HODOengine/HODO_API.h"
#include "Weapon.h"

/// <summary>
/// �÷��̾� ���� - ������
/// [������ �־�� �ϴ� ����]
/// index, ų ��, ���� ��, ��, ü��, ���� �� ����, ���� �Ѿ� ��, ���� ��, �ӵ�, ���� ����, 
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
	// state ���߿� �߰�

	int _kill;
	int _death;

	eWeaponType _weapon;
	int _bulletCount;

	int _coin;
};

