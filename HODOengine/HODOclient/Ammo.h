#pragma once
#include  "../HODOengine/HODO_API.h"

class PlayerMove;

class Ammo : public HDData::Script
{
public:
	Ammo();

	virtual void Start() override;
	virtual void Update() override;

	PlayerMove* playerMove;

private:
	HDData::TextUI* _defaultAmmo;
	HDData::TextUI* _remainningAmmo;
	int _bulletCount;
};
