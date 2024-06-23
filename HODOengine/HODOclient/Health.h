#pragma once
#include  "../HODOengine/HODO_API.h"

class PlayerMove;

class Health : public HDData::Script
{
public:
	Health();

	virtual void Start() override;
	virtual void Update() override;

private:
	HDData::TextUI* _playerHealth;
	int& _healthPoint;
};
