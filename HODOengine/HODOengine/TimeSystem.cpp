#include "TimeSystem.h"

namespace hodoEngine
{


	void TimeSystem::Initialize()
	{
		QueryPerformanceFrequency((LARGE_INTEGER*) & _frequency);
		QueryPerformanceCounter((LARGE_INTEGER*)&_startTime);
		
		// 미리 역수를 취해 매 프레임당 곱셈 연산을 하도록...
		_timeScale = 1.f / _frequency;
	}

	void TimeSystem::Update()
	{
		QueryPerformanceCounter((LARGE_INTEGER*)&_stopTime);
		_deltaTime = (((LARGE_INTEGER*)&_stopTime)->QuadPart - ((LARGE_INTEGER*)&_startTime)->QuadPart) * _timeScale;
		_startTime = _stopTime;
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
