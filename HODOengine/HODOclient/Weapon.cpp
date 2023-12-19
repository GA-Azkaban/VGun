#include "Weapon.h"

Weapon::Weapon(WeaponBuilder* builder)
{

}

void WeaponBuilder::SetGunType(eWeaponType type)
{
	weapon.type = type;
}

void WeaponBuilder::SetBulletCount(int count)
{
	weapon.bullet = count;
}

void WeaponBuilder::SetReloadTime(float time)
{
	weapon.reloadTime = time;
}

void WeaponBuilder::SetGunDamage(float damage)
{
	weapon.damage = damage;
}

void WeaponBuilder::SetGunPrice(int price)
{
	weapon.price = price;
}
