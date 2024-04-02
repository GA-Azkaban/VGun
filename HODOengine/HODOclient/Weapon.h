#pragma once
#include "../HODOengine/HODO_API.h"

enum class eWeaponType
{
	HANDGUN,
	SHOTGUN,
	ASSAULT_RIFLE1,
	ASSAULT_RIFLE2,
	ASSAULT_RIFLE3,
	SNIPER
};

struct weaponData
{
	std::string weaponName;
	int			bulletMax;
	int			dealBody;
	int			dealHead;
	int			reloadTime;
};

class Weapon : public HDData::Script
{
public:
	void Update() override;
};
