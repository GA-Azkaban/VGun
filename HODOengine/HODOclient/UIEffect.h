#pragma once
#include "Timer.h"
#include "../HODOengine/HODO_API.h"

class UIEffect : public HDData::Script
{
public:
	//UIEffect(eUIEFFECT type, Vector2 endScale, float time);
	//~UIEffect();

	void Start() override;
	void Update() override;

	void Play();

private:
	void ScaleUpdate();

	Timer* _timer;
};
