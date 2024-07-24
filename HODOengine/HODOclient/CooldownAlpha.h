#pragma once
#include "../HODOengine/HODO_API.h"
#include "Timer.h"

class CooldownAlpha : public HDData::Script
{
public:
	CooldownAlpha();

	virtual void Start() override;
	virtual void Update() override;

	void SetTimerOn();

private:
	HDData::ImageUI* _cooldownImage;
	Timer _cooldownTimer;
	HDData::AnimationCurve _curve;
	Vector3 _originalPosition;
};
