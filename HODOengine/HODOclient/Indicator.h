#pragma once
#include "../HODOengine/HODO_API.h"
#include "Timer.h"

class Indicator : public HDData::Script
{
public:
	Indicator();
	static Indicator* CreateIndicator();

	virtual void Update() override;

	void SetEffectOn(Vector3 targetPos);

private:
	HDData::ImageUI* _indicatorImage;
	Timer _indicatorTimer;
	Vector3 _targetPos;
};
