#include "Timer.h"
#include "../HODOengine/HODO_API.h"

Timer::Timer()
	: isActive(false), isRepeat(false), elapsed(0.0f), duration(1.0f),
	onExpiration(), onUpdate()
{

}

void Timer::Start()
{
	isActive = true;
	elapsed = 0.0f;
}

void Timer::Update()
{
	if (isActive)
	{
		elapsed += API::GetDeltaTime();

		if (elapsed > duration)
		{
			elapsed = duration;
		}

		if (onUpdate)
		{
			onUpdate(elapsed / duration);
		}

		if (elapsed == duration)
		{
			if (isRepeat)
			{
				elapsed = 0.0f;
			}
			else
			{
				isActive = false;
			}
			onExpiration();
		}
	}
}

void Timer::Stop()
{
	isActive = false;
}

bool Timer::IsActive()
{
	return isActive;
}

float Timer::GetElapsedTime()
{
	return elapsed;
}
