#include "Player.h"

void Player::Start()
{
	// 플레이어에 대한 데이터를 세팅
	_kill = 0;
	_death = 0;
	_weapon = eWeaponType::HANDGUN;
	_isDie = false;
}

void Player::Update()
{
	
}

void Player::LateUpdate()
{

}

int Player::GetPlayerIndex()
{
	return _playerIndex;
}

int Player::GetTeamIndex()
{
	return _teamID;
}

int Player::GetHealth()
{
	return _health;
}

int Player::GetSpeed()
{
	return _speed;
}

int Player::GetKillCount()
{
	return _kill;
}

int Player::GetDeathCount()
{
	return _death;
}

eWeaponType Player::GetWeaponNow()
{
	return _weapon;
}

int Player::GetBulletCount()
{
	return _bulletCount;
}

void Player::ChangeWeapon(eWeaponType type)
{
	_weapon = type;
}
