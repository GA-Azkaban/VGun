#pragma once
#include "Timer.h"
#include "../HODOengine/HODO_API.h"

class UIEffect : public HDData::Script
{
public:
	UIEffect(Vector2 destScale, float time, bool isFade, float power);
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
	bool _isFade;
	float _time;
	float _power;
	Vector2 _start;
	Vector2 _originPos;
	Vector2 _originScale;
	Timer* _scaleTimer;
	Timer* _shakeTimer;
};
