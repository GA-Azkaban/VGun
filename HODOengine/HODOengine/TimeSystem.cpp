#include "TimeSystem.h"

namespace HDEngine
{


	void TimeSystem::Initialize()
	{
		QueryPerformanceFrequency((LARGE_INTEGER*) & _frequency);
		QueryPerformanceCounter((LARGE_INTEGER*)&_startTime);
	}

	void TimeSystem::Update()
	{
		QueryPerformanceCounter(&_stopTime);
		_deltaTime = static_cast<float>(_stopTime.QuadPart - _startTime.QuadPart) / static_cast<double>(_frequency.QuadPart);
		QueryPerformanceCounter(&_startTime);
	}

	float TimeSystem::GetDeltaTime()
	{
		return _deltaTime;
	}

	int TimeSystem::GetFramePerSecond()
	{
		return 1.f / _deltaTime;
	}

}
