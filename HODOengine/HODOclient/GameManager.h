#pragma once
#include <unordered_map>

#include "../HODOengine/HODO_API.h"
#include "Weapon.h"

class GameManager : public HDData::Script
{
public:
	GameManager* GetInstance();

private:
	static GameManager* instance;
	GameManager();
	~GameManager() = default;

public:
	void InitGame();

public:
	std::unordered_map<eWeaponType, const weaponData*> GetWeapons();

private:
	void SetWeaponData();
	std::unordered_map<eWeaponType, const weaponData*> _weapons;
};

