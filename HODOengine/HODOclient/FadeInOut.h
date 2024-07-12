#pragma once
#include "../HODOengine/HODO_API.h"
#include "Timer.h"

class FadeInOut : public HDData::Script
{
private:
	FadeInOut(); 
	~FadeInOut() = default;
	HDData::GameObject* _fadeImage = nullptr;


	Timer _fadeInTimer;
	Timer _fadeOutTimer;

public:
	void Start() override;
	void Update() override;

	void FadeIn();
	void FadeOut();
	bool GetComplete();
};
