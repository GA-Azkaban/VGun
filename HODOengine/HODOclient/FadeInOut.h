#pragma once
#include "../HODOengine/HODO_API.h"
#include "Timer.h"

class FadeInOut : public HDData::Script
{
public:
	static FadeInOut& Instance();

private:
	static FadeInOut* _instance;
	FadeInOut(); 
	~FadeInOut() = default;
	HDData::GameObject* _fadeCanvas;

	Timer _fadeInTimer;
	Timer _fadeOutTimer;

public:
	void Start() override;
	void Update() override;

	void FadeIn();
	void FadeOut();
	bool GetComplete();
};
