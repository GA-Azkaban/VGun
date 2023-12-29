#include "Weapon.h"

WeaponBuilder::WeaponBuilder()
{
	_builder = new WeaponData();
}

void WeaponBuilder::SetGunType(eWeaponType type)
{
	_builder->type = type;
}

void WeaponBuilder::SetBulletCount(int count)
{
	_builder->bullet = count;
}

void WeaponBuilder::SetReloadTime(float time)
{
	_builder->reloadTime = time;
}

void WeaponBuilder::SetGunDamage(float damage)
{
	_builder->damage = damage;
}

void WeaponBuilder::SetGunPrice(int price)
{
	_builder->price = price;
}

eWeaponType WeaponBuilder::GetWeaponType()
{
	return _builder->type;
}

int WeaponBuilder::GetBulletCount()
{
	return _builder->bullet;
}

float WeaponBuilder::GetReloadTime()
{
	return _builder->reloadTime;
}

float WeaponBuilder::GetDamage()
{
	return _builder->damage;
}

int WeaponBuilder::GetPrice()
{
	return _builder->price;
}

Weapon::Weapon(WeaponBuilder* builder)
{
	_type = builder->GetWeaponType();
	_bulletCapacity = builder->GetBulletCount();
	_reloadTime = builder->GetReloadTime();
	_damage = builder->GetDamage();
	_price = builder->GetPrice();
}

void Weapon::Start()
{

}

void Weapon::Update()
{

}

void Weapon::LateUpdate()
{

}

