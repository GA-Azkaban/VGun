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
		LARGE_INTEGER			_frequency;
		LARGE_INTEGER			_startTime;
		LARGE_INTEGER			_stopTime;
		float					_deltaTime;

	};
}


