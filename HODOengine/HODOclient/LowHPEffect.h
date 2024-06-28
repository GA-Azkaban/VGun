#pragma once
#include "../HODOengine/HODO_API.h"
#include "Timer.h"

class PlayerInfo;

class LowHPEffect : public HDData::Script
{
public:
	LowHPEffect();

	virtual void Start() override;
	virtual void Update() override;

	//PlayerInfo* info;

private:
	HDData::ImageUI* _lowHPEffectImage;
	Timer _lowHPEffectTimer;
	float _effectColorThreshold;
	HDData::AnimationCurve _curve;
	static const int maxHP = 100;
};
