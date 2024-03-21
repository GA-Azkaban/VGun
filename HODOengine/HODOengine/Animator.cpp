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
		GetGameObject()->GetComponent<HDData::SkinnedMeshRenderer>()->PlayAnimation(_animationController->GetCurrentState()->motion);
	}

	void Animator::Update()
	{
		if (_animationController == nullptr) return;

		_animationController->Update();
		_current = _animationController->GetCurrentStateName();
		_prev = _animationController->GetPrevStateName();

		if (_current != _prev)
		{
			GetGameObject()->GetComponent<HDData::SkinnedMeshRenderer>()->PlayAnimation(_animationController->GetCurrentState()->motion);
		}
	}

	void Animator::SetAnimationController(AnimationController* controller)
	{
		_animationController = controller;
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
