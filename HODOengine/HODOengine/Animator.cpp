﻿#include "Animator.h"
#include "SkinnedMeshRenderer.h"
#include "GameObject.h"

namespace HDData
{
	Animator::Animator()
		: _animationController(nullptr), _engageIng(false)
	{

	}

	Animator::~Animator()
	{

	}

	void Animator::Start()
	{
		if (_animationController == nullptr) return;

		_animationController->Start();
		_meshRenderer = GetGameObject()->GetComponentInChildren<HDData::SkinnedMeshRenderer>();
		_meshRenderer->PlayAnimationUpper(_animationController->GetCurrentState()->_upperMotion, true);
		_meshRenderer->PlayAnimationLower(_animationController->GetCurrentState()->_lowerMotion, true);
	}

	void Animator::Update()
	{
		if (_animationController == nullptr) return;

		_animationController->Update();

		if (!_animationController->GetMotionBuffer().empty())
		{
			_meshRenderer->PlayAnimationUpper(_animationController->GetAllStates().find(_animationController->GetMotionBuffer().front().c_str())->second->_upperMotion, false);
			_meshRenderer->PlayAnimationLower(_animationController->GetAllStates().find(_animationController->GetMotionBuffer().front().c_str())->second->_lowerMotion, false);
			_animationController->GetMotionBuffer().pop_back();
			_engageIng = true;
		}

		if (_engageIng && _meshRenderer->IsAnimationEnd())
		{
			_engageIng = false;
			_animationController->_isStateChange = true;
		}
		
		if(_animationController->IsStateChange() && !_engageIng)
		{
			if (_animationController->GetCurrentState()->GetIsLoop())
			{
				_meshRenderer->PlayAnimationUpper(_animationController->GetCurrentState()->_upperMotion, true);
				_meshRenderer->PlayAnimationLower(_animationController->GetCurrentState()->_lowerMotion, true);
			}
			else
			{
				_meshRenderer->PlayAnimationUpper(_animationController->GetCurrentState()->_upperMotion, false);
				_meshRenderer->PlayAnimationLower(_animationController->GetCurrentState()->_lowerMotion, false);
			}
		}

		if (!(_animationController->GetCurrentState()->GetIsLoop()) && _meshRenderer->IsAnimationEnd())
		{
			_animationController->SetCurrentState(_animationController->GetPrevStateName());
			_meshRenderer->PlayAnimationUpper(_animationController->GetCurrentState()->_upperMotion, true);
			_meshRenderer->PlayAnimationLower(_animationController->GetCurrentState()->_lowerMotion, true);
		}
	}

	void Animator::SetAnimationController(AnimationController* controller)
	{
		_animationController = controller;
	}

	std::string Animator::GetCurrentState()
	{
		return _animationController->GetCurrentStateName();
	}

	void Animator::SetCurrentState(std::string stateName)
	{
		_animationController->SetCurrentState(stateName);
	}

	void Animator::SetFloat(std::string name, float val)
	{
		_animationController->SetFloatParam(name, val);
	}

	void Animator::SetInt(std::string name, int val)
	{
		_animationController->SetIntParam(name, val);
	}

	void Animator::SetBool(std::string name, bool val)
	{
		_animationController->SetBoolParam(name, val);
	}

	void Animator::SetTrigger(std::string name)
	{
		_animationController->SetTriggerParam(name);
	}

}
