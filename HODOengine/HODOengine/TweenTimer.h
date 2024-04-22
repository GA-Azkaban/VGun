#pragma once
#include <functional>

namespace HDEngine
{
	class TweenTimer
	{
	public:
		TweenTimer();
		~TweenTimer();

	public:
		void Start();
		void Update();

	public:
		float _time;
		float _delayTime;
		float _duration;
		std::function<void()> _play;

		bool _isActive;
	};
}


