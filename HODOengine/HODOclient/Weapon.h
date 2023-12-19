#pragma once
#include "../HODOengine/HODO_API.h"

/// <summary>
/// �� ������ - ������
/// [���� �� ��]
/// �� Ÿ��, ��ź ��, ���� �ð�, ź ������, �� ����
/// </summary>

enum class eWeaponType
{
	HANDGUN,
	SHOTGUN,
	ASSAULT_RIFLE1,
	ASSAULT_RIFLE2,
	ASSAULT_RIFLE3,
	SNIPER
};

// TODO) �� ���� �����
struct WeaponData
{
	eWeaponType type;
	int			bullet;
	float		reloadTime;
	float		damage;
	int			price;
};

// ���� Ŭ����
class WeaponBuilder
{
public:
	WeaponBuilder();
	void SetGunType(eWeaponType type);
	void SetBulletCount(int count);
	void SetReloadTime(float time);
	void SetGunDamage(float damage);
	void SetGunPrice(int price);

private:
	WeaponData weapon;
};


// Weapon
class Weapon : public HDData::Script
{
public:
	Weapon(WeaponBuilder* builder);

private:
	int bullet;
	float reloadTime;
	float damage;
	int price;
};

