#include "Animation.h"

namespace HDData
{


	Animation::Animation()
		: _exitTime(0.f), _isLoop(true)
	{

	}

	Animation::~Animation()
	{

	}

	bool Animation::GetIsLoop()
	{
		return _isLoop;
	}

	void Animation::SetIsLoop(bool isLoop)
	{
		_isLoop = isLoop;
	}

	float Animation::GetExitTime()
	{
		return _exitTime;
	}

	void Animation::SetExitTime(float time)
	{
		_exitTime = time;
	}

	bool Animation::IsAnimationEnd()
	{
		if (_isLoop) return false;
		else
		{

		}
	}

}
