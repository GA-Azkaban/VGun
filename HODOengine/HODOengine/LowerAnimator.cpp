#include "LowerAnimator.h"
#include "SkinnedMeshRenderer.h"
#include "GameObject.h"

namespace HDData
{

	LowerAnimator::LowerAnimator()
		: _animationController(nullptr), _engageIng(false)
	{

	}

	LowerAnimator::~LowerAnimator()
	{

	}

	void LowerAnimator::Start()
	{
		if (_animationController == nullptr) return;

		_animationController->Start();
		_meshRenderer->PlayAnimationLower(_animationController->GetCurrentState()->_motion, true, 0.1, _animationController->GetCurrentState()->_hasExitTime, _animationController->GetCurrentState()->_exitTime);
	}

	void LowerAnimator::Update()
	{
		if (_animationController == nullptr) return;

		_animationController->Update();

		if (!_animationController->GetMotionBuffer().empty())
		{
			_meshRenderer->PlayAnimationLower(_animationController->GetAllStates().find(_animationController->GetMotionBuffer().front().c_str())->second->_motion, false, 0.075, _animationController->GetCurrentState()->_hasExitTime, _animationController->GetCurrentState()->_exitTime);
			_animationController->GetMotionBuffer().pop_back();
			_engageIng = true;
		}

		if (_engageIng && _meshRenderer->IsLowerAnimationEnd())
		{
			_engageIng = false;
			_animationController->_isStateChange = true;
		}

		if (_animationController->IsStateChange() && !_engageIng)
		{
			if (_animationController->GetCurrentState()->GetIsLoop() && _animationController->GetCurrentState()->_motion != "")
			{
				_meshRenderer->PlayAnimationLower(_animationController->GetCurrentState()->_motion, true, 0.1, _animationController->GetCurrentState()->_hasExitTime, _animationController->GetCurrentState()->_exitTime);
			}
			else if (_animationController->GetCurrentState()->_motion != "")
			{
				_meshRenderer->PlayAnimationLower(_animationController->GetCurrentState()->_motion, false, 0.1, _animationController->GetCurrentState()->_hasExitTime, _animationController->GetCurrentState()->_exitTime);
			}
		}

		if (!(_animationController->GetCurrentState()->GetIsLoop()) && _meshRenderer->IsLowerAnimationEnd())
		{
			_animationController->SetCurrentState(_animationController->GetPrevStateName());
			_meshRenderer->PlayAnimationLower(_animationController->GetCurrentState()->_motion, true, 0.1, _animationController->GetCurrentState()->_hasExitTime, _animationController->GetCurrentState()->_exitTime);
		}
	}

	void LowerAnimator::SetAnimationController(AnimationController* controller)
	{
		_animationController = controller;
	}

	void LowerAnimator::SetMeshRenderer(SkinnedMeshRenderer* renderer)
	{
		_meshRenderer = renderer;
	}

	std::string LowerAnimator::GetCurrentState()
	{
		return _animationController->GetCurrentStateName();
	}

	void LowerAnimator::SetCurrentState(std::string stateName)
	{
		_animationController->SetCurrentState(stateName);
	}

	void LowerAnimator::SetFloat(std::string name, float val)
	{
		_animationController->SetFloatParam(name, val);
	}

	void LowerAnimator::SetInt(std::string name, int val)
	{
		_animationController->SetIntParam(name, val);
	}

	void LowerAnimator::SetBool(std::string name, bool val)
	{
		_animationController->SetBoolParam(name, val);
	}

	void LowerAnimator::SetTrigger(std::string name)
	{
		_animationController->SetTriggerParam(name);
	}

}
