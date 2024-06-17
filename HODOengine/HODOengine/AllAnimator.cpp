#include "AllAnimator.h"
#include "SkinnedMeshRenderer.h"
#include "GameObject.h"

namespace HDData
{


	AllAnimator::AllAnimator()
		: _animationController(nullptr), _engageIng(false)
	{

	}

	AllAnimator::~AllAnimator()
	{

	}

	void AllAnimator::Start()
	{
		if (_animationController == nullptr) return;

		_animationController->Start();
		_meshRenderer->PlayAnimation(_animationController->GetCurrentState()->_motion, true, 0.1, _animationController->GetCurrentState()->_hasExitTime, _animationController->GetCurrentState()->_exitTime);
	}

	void AllAnimator::Update()
	{
		if (_animationController == nullptr) return;

		_animationController->Update();

		if (!_animationController->GetMotionBuffer().empty())
		{
			_meshRenderer->PlayAnimation(_animationController->GetAllStates().find(_animationController->GetMotionBuffer().front().c_str())->second->_motion, false, 0.1, _animationController->GetCurrentState()->_hasExitTime, _animationController->GetCurrentState()->_exitTime);
			_animationController->GetMotionBuffer().pop_back();
			_engageIng = true;
		}

		if (_engageIng && _meshRenderer->IsAnimationEnd())
		{
			_engageIng = false;
			_animationController->_isStateChange = true;
		}

		if (_animationController->IsStateChange() && !_engageIng)
		{
			if (_animationController->GetCurrentState()->_isAnyState)
			{
				_meshRenderer->PlayAnimationAtOnce(_animationController->GetCurrentState()->_motion, false, 0.1, _animationController->GetCurrentState()->_hasExitTime, _animationController->GetCurrentState()->_exitTime);
			}
			else if (_animationController->GetCurrentState()->GetIsLoop() && _animationController->GetCurrentState()->_motion != "")
			{
				_meshRenderer->PlayAnimation(_animationController->GetCurrentState()->_motion, true, 0.1, _animationController->GetCurrentState()->_hasExitTime, _animationController->GetCurrentState()->_exitTime);
			}
			else if (_animationController->GetCurrentState()->_motion != "")
			{
				_meshRenderer->PlayAnimation(_animationController->GetCurrentState()->_motion, false, 0.1, _animationController->GetCurrentState()->_hasExitTime, _animationController->GetCurrentState()->_exitTime);
				//_animationController->SetCurrentState(_animationController->GetPrevStateName());
			}
		}

		if (!(_animationController->GetCurrentState()->GetIsLoop()) && _meshRenderer->IsAnimationEnd())
		{
			_animationController->SetCurrentState(_animationController->GetPrevStateName());
			_meshRenderer->PlayAnimation(_animationController->GetCurrentState()->_motion, true, 0.1, _animationController->GetCurrentState()->_hasExitTime, _animationController->GetCurrentState()->_exitTime);
		}
	}

	void AllAnimator::SetAnimationController(AnimationController* controller)
	{
		_animationController = controller;
	}

	void AllAnimator::SetMeshRenderer(SkinnedMeshRenderer* renderer)
	{
		_meshRenderer = renderer;
	}

	std::string AllAnimator::GetCurrentState()
	{
		return _animationController->GetCurrentStateName();
	}

	void AllAnimator::SetCurrentState(std::string stateName)
	{
		_animationController->SetCurrentState(stateName);
	}

	void AllAnimator::SetFloat(std::string name, float val)
	{
		_animationController->SetFloatParam(name, val);
	}

	void AllAnimator::SetInt(std::string name, int val)
	{
		_animationController->SetIntParam(name, val);
	}

	void AllAnimator::SetBool(std::string name, bool val)
	{
		_animationController->SetBoolParam(name, val);
	}

	void AllAnimator::SetTrigger(std::string name)
	{
		_animationController->SetTriggerParam(name);
	}

}
