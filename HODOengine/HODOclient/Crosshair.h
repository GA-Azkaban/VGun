﻿#pragma once
#include "../HODOengine/HODO_API.h"
#include "Timer.h"

class PlayerMove;

class Crosshair : public HDData::Script
{
public:
	Crosshair();

	virtual void Start() override;
	virtual void Update() override;

	PlayerMove* playerMove;

private:
	HDData::ImageUI* _defaultCrosshair;
	HDData::ImageUI* _hitCrosshair;
	HDData::ImageUI* _criticalCrosshair;
	Timer _hitCrosshairTimer;
	Timer _criticalCrosshairTimer;
};
