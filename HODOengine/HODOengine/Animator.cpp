#include "Animator.h"
#include "GameObject.h"
#include "SkinnedMeshRenderer.h"

namespace HDData
{
	Animator::Animator()
	{
		_upper = new UpperAnimator();
		_lower = new LowerAnimator();
		_all = new AllAnimator();
	}

	Animator::~Animator()
	{
		delete _upper;
		delete _lower;
		delete _all;
	}

	void Animator::Start()
	{
		auto MR = GetGameObject()->GetComponentInChildren<HDData::SkinnedMeshRenderer>();
		_upper->SetMeshRenderer(MR);
		_upper->Start();
		_lower->SetMeshRenderer(MR);
		_lower->Start();
		_all->SetMeshRenderer(MR);
		_all->Start();
	}

	void Animator::Update()
	{
		_upper->Update();
		_lower->Update();
		_all->Update();
	}

	void Animator::SetUpperAnimationController(AnimationController* controller)
	{
		_upper->SetAnimationController(controller);
	}

	void Animator::SetLowerAnimationController(AnimationController* controller)
	{
		_lower->SetAnimationController(controller);
	}

	void Animator::SetAllAnimationController(AnimationController* controller)
	{
		_all->SetAnimationController(controller);
	}

	HDData::UpperAnimator* Animator::GetUpperAC()
	{
		return _upper;
	}

	LowerAnimator* Animator::GetLowerAC()
	{
		return _lower;
	}

	HDData::AllAnimator* Animator::GetAllAC()
	{
		return _all;
	}

}
