#pragma once
#include "../HODOengine/HODO_API.h"
#include "Timer.h"

class CooldownText : public HDData::Script
{
public:
	CooldownText();

	virtual void Start() override;
	virtual void Update() override;

	void SetTimerOn();

private:
	HDData::TextUI* _cooldownText;
	Timer _cooldownTimer;
};
