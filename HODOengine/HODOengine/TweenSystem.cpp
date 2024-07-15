#include "TweenSystem.h"
#include "Tween.h"
#include "TweenTimer.h"

void HDEngine::TweenSystem::Initialize()
{

}

void HDEngine::TweenSystem::Update()
{
	for (auto& tween : _dotweens)
	{
		if (tween->_timer->_isActive)
		{
			tween->_timer->Update();
		}

		if (tween->_timer->_isComplete)
		{
			if (tween->GetCompleteFunc() != nullptr)
			{
				tween->_onCompleteFunc();
				tween->_timer->_isComplete = false;
			}
			//_over.push_back(tween);
		}
	}

	if (_over.empty()) return;

	auto findEndTween = _over.begin();
	for (auto& tween : _over)
	{
		auto iter = std::find(_dotweens.begin(), _dotweens.end(), *findEndTween);
		if (iter != _dotweens.end())
		{
			_dotweens.erase(iter);
		}
		++findEndTween;
	}

	_over.clear();
}

void HDEngine::TweenSystem::Finalize()
{

}

HDData::Tween& HDEngine::TweenSystem::CreateTween()
{
	HDData::Tween* temp = new HDData::Tween();
	_dotweens.push_back(temp);
	return *_dotweens.back();
}
