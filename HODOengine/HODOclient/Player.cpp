#include "Player.h"

void Player::Start()
{

	_kill = 0;
	_death = 0;
	_weapon = eWeaponType::HANDGUN;
	_weaponList.clear();
	_coin = 0;
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
	return _teamIndex;
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

std::vector<eWeaponType> Player::GetWeaponList()
{
	return _weaponList;
}

int Player::GetBulletCount()
{
	return _bulletCount;
}

int Player::GetCoinCount()
{
	return _coin;
}
