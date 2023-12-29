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


struct WeaponData
{
	eWeaponType type;
	int			bullet;
	float		reloadTime;
	float		damage;
	int			price;
};

// 무기 빌더 클래스
class WeaponBuilder
{
public:
	WeaponBuilder();
	void SetGunType(eWeaponType type);
	void SetBulletCount(int count);
	void SetReloadTime(float time);
	void SetGunDamage(float damage);
	void SetGunPrice(int price);

	eWeaponType GetWeaponType();
	int GetBulletCount();
	float GetReloadTime();
	float GetDamage();
	int GetPrice();

private:
	WeaponData* _builder;
};


// Weapon
class Weapon : public HDData::Script
{
public:
	Weapon(WeaponBuilder* builder);

public:
	void Start() override;
	void Update() override;
	void LateUpdate() override;

private:
	eWeaponType _type;
	int _bulletCapacity;
	float _reloadTime;
	float _damage;
	int _price;
};

