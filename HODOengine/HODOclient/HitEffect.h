#pragma once
#include "../HODOengine/HODO_API.h"
#include "Timer.h"

class GameManager;

class HitEffect : public HDData::Script
{
public:
	HitEffect();

	virtual void Start() override;
	virtual void Update() override;

	void SetEffectOn();

private:
	HDData::ImageUI* _hitEffectImage;
	Timer _hitEffectTimer;
};
