#pragma once
#include "../HODOengine/HODO_API.h"

class UpdatePlayerInfo : public HDData::Script
{
public:
	UpdatePlayerInfo(HDData::GameObject* name, HDData::GameObject* health, HDData::GameObject* bulletCount);
	~UpdatePlayerInfo();

	void Start() override;
	void Update() override;

private:
	HDData::TextUI* _name;
	HDData::TextUI* _health;
	HDData::GameObject* _bullet;
};

