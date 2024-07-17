#pragma once
#include "Timer.h"
#include "../HODOengine/HODO_API.h"

class UIEffect : public HDData::Script
{
public:
	UIEffect(Vector2 destScale, float time);
	~UIEffect();

	void Start() override;
	void Update() override;

	void Play();

private:
	void ScaleUpdate();
	void Shake();
	void EndEffect();

	HDData::ImageUI* _ui;
	bool _isPlay;
	bool _isShake;
	bool _isLeft;
	float _time;
	Vector2 _start;
	Timer* _scaleTimer;
	Timer* _shakeTimer;
};
