﻿#pragma once
#include "../HODOengine/HODO_API.h"
#include "Timer.h"
#include <vector>

class Indicator : public HDData::Script
{
public:
	Indicator();
	static Indicator* CreateIndicator();

	virtual void Update() override;

	void SetEffectOn(Vector3 targetPos);

private:
	float CalculateDegreeBetweenTwoVectors(Vector3 v1, Vector3 v2);
	HDData::ImageUI* _indicatorImage;
	Timer _indicatorTimer;
	Vector3 _targetPos;
};
