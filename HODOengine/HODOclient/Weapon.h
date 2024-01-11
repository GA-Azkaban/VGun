#pragma once

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

