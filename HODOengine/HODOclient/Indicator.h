#pragma once
#include "../HODOengine/HODO_API.h"
#include "Timer.h"
#include <vector>

class Indicator : public HDData::Script
{
public:
	static Indicator* CreateIndicator();

	virtual void Update() override;

	void SetEffectOn(Vector3 targetPos);

private:
	Indicator();
	HDData::ImageUI* _indicatorImage;
	Timer _indicatorTimer;
	Vector3 _targetPos;
};
