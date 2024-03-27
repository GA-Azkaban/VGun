#include "Animator.h"
#include "SkinnedMeshRenderer.h"
#include "GameObject.h"

namespace HDData
{
	Animator::Animator()
		: _animationController(nullptr)
	{

	}

	Animator::~Animator()
	{

	}

	void Animator::Start()
	{
		if (_animationController == nullptr) return;

		_animationController->Start();

		GetGameObject()->GetComponent<HDData::SkinnedMeshRenderer>()->PlayAnimation(_animationController->GetCurrentState()->motion, true);
	}

	void Animator::Update()
	{
		if (_animationController == nullptr) return;

		_animationController->Update();

		if (_animationController->IsStateChange())
		{
			if (_animationController->GetCurrentState()->GetIsLoop())
			{
				GetGameObject()->GetComponent<HDData::SkinnedMeshRenderer>()->PlayAnimation(_animationController->GetCurrentState()->motion, true);
			}
			else
			{
				GetGameObject()->GetComponent<HDData::SkinnedMeshRenderer>()->PlayAnimation(_animationController->GetCurrentState()->motion, false);
			}
		}

		if (!(_animationController->GetCurrentState()->GetIsLoop()) && GetGameObject()->GetComponent<HDData::SkinnedMeshRenderer>()->IsAnimationEnd())
		{
			_animationController->SetCurrentState(_animationController->GetPrevStateName());
			GetGameObject()->GetComponent<HDData::SkinnedMeshRenderer>()->PlayAnimation(_animationController->GetCurrentState()->motion, true);
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
