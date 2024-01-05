#include "GameManager.h"

GameManager* GameManager::GetInstance()
{
	if (instance == nullptr)
	{
		instance = new GameManager;
	}

	return instance;
}

GameManager* GameManager::instance = nullptr;

GameManager::GameManager()
{

}

void GameManager::InitGame()
{


	// 모든 무기에 대한 데이터 설정
	SetWeaponData();
}

std::unordered_map<eWeaponType, const weaponData*> GameManager::GetWeapons()
{
	return _weapons;
}

void GameManager::SetWeaponData()
{
	weaponData* handGun = new weaponData;
	_weapons.insert({eWeaponType::HANDGUN, handGun});

	weaponData* shotGun = new weaponData;
	_weapons.insert({ eWeaponType::SHOTGUN, shotGun });

	weaponData* AR1 = new weaponData;
	_weapons.insert({ eWeaponType::ASSAULT_RIFLE1, AR1 });

	weaponData* AR2 = new weaponData;
	_weapons.insert({ eWeaponType::ASSAULT_RIFLE2, AR2 });

	weaponData* AR3 = new weaponData;
	_weapons.insert({ eWeaponType::ASSAULT_RIFLE3, AR3 });

	weaponData* sniper = new weaponData;
	_weapons.insert({ eWeaponType::SNIPER, sniper });
}
