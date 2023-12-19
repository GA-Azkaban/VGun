#pragma once
#include "../HODOengine/HODO_API.h"

/// <summary>
/// 총 데이터 - 오수안
/// [들어가야 할 것]
/// 총 타입, 장탄 수, 장전 시간, 탄 데미지, 총 가격
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

// TODO) 총 빌더 만들기
struct WeaponData
{
	eWeaponType type;
	int			bullet;
	float		reloadTime;
	float		damage;
	int			price;
};

// 빌더 클래스
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

