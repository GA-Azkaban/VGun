#include "TweenTimer.h"
#include "TimeSystem.h"

namespace HDEngine
{
	TweenTimer::TweenTimer()
		: _time(0.f), _isActive(false), _duration(0.f), _delayTime(0.f)
	{

	}

	TweenTimer::~TweenTimer()
	{

	}

	void TweenTimer::Start()
	{
		_isActive = true;
	}

	void TweenTimer::Update()
	{
		if (_isActive)
		{
			_time += TimeSystem::Instance().GetDeltaTime();

			if (_time > _duration)
			{
				_time = _duration;
			}

			if (_time > 0)
			{
				_play();
			}

			if (_time == _duration)
			{
				_isActive = false;
			}
		}
	}
}
