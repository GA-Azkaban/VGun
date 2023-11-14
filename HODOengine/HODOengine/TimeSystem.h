#pragma once
#include "Singleton.h"

#include <windows.h>



namespace hodoEngine
{
	class TimeSystem : public Singleton<TimeSystem>
	{
		friend Singleton;

	private:
		TimeSystem() = default;

	public:
		void Initialize();
		void Update();
		
	public:
		float GetDeltaTime();
		int GetFramePerSecond();

	private:
		float			_frequency;
		float			_startTime;
		float			_stopTime;
		float			_timeScale;
		float			_deltaTime;

	};
}


