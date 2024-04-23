#include "UpperAnimator.h"
#include "SkinnedMeshRenderer.h"
#include "GameObject.h"

namespace HDData
{
	UpperAnimator::UpperAnimator()
		: _animationController(nullptr), _engageIng(false)
	{

	}

	UpperAnimator::~UpperAnimator()
	{

	}

	void UpperAnimator::Start()
	{
		if (_animationController == nullptr) return;

		_animationController->Start();
		_meshRenderer->PlayAnimationUpper(_animationController->GetCurrentState()->_motion, true);
	}

	void UpperAnimator::Update()
	{
		if (_animationController == nullptr) return;

		_animationController->Update();

		if (!_animationController->GetMotionBuffer().empty())
		{
			_meshRenderer->PlayAnimationUpper(_animationController->GetAllStates().find(_animationController->GetMotionBuffer().front().c_str())->second->_motion, false);
			_animationController->GetMotionBuffer().pop_back();
			_engageIng = true;
		}

		if (_engageIng && _meshRenderer->IsUpperAnimationEnd())
		{
			_engageIng = false;
			_animationController->_isStateChange = true;
		}
		
		if(_animationController->IsStateChange() && !_engageIng)
		{
			if (_animationController->GetCurrentState()->GetIsLoop() && _animationController->GetCurrentState()->_motion != "")
			{
				_meshRenderer->PlayAnimationUpper(_animationController->GetCurrentState()->_motion, true);
			}
			else if(_animationController->GetCurrentState()->_motion != "")
			{
				_meshRenderer->PlayAnimationUpper(_animationController->GetCurrentState()->_motion, false);
			}
		}

		if (!(_animationController->GetCurrentState()->GetIsLoop()) && _meshRenderer->IsUpperAnimationEnd())
		{
			_animationController->SetCurrentState(_animationController->GetPrevStateName());
			_meshRenderer->PlayAnimationUpper(_animationController->GetCurrentState()->_motion, true);
		}
	}

	void UpperAnimator::SetAnimationController(AnimationController* controller)
	{
		_animationController = controller;
	}

	void UpperAnimator::SetMeshRenderer(SkinnedMeshRenderer* renderer)
	{
		_meshRenderer = renderer;
	}

	std::string UpperAnimator::GetCurrentState()
	{
		return _animationController->GetCurrentStateName();
	}

	void UpperAnimator::SetCurrentState(std::string stateName)
	{
		_animationController->SetCurrentState(stateName);
	}

	void UpperAnimator::SetFloat(std::string name, float val)
	{
		_animationController->SetFloatParam(name, val);
	}

	void UpperAnimator::SetInt(std::string name, int val)
	{
		_animationController->SetIntParam(name, val);
	}

	void UpperAnimator::SetBool(std::string name, bool val)
	{
		_animationController->SetBoolParam(name, val);
	}

	void UpperAnimator::SetTrigger(std::string name)
	{
		_animationController->SetTriggerParam(name);
	}

}
