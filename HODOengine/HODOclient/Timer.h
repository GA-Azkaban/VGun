#pragma once
#include <functional>

class Timer
{
public:
	Timer();
	void Start();
	void Update();

	bool IsActive();
	float GetElapsedTime();

	bool isRepeat;
	float duration;
	std::function<void()> onExpiration;
	std::function<void(float progress)> onUpdate;

private:
	bool isActive;
	float elapsed;
};
