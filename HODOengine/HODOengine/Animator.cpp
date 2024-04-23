#include "Animator.h"
#include "GameObject.h"
#include "SkinnedMeshRenderer.h"

namespace HDData
{
	Animator::Animator()
	{
		_upper = new UpperAnimator();
		_lower = new LowerAnimator();
	}

	Animator::~Animator()
	{
		delete _upper;
		delete _lower;
	}

	void Animator::Start()
	{
		auto MR = GetGameObject()->GetComponentInChildren<HDData::SkinnedMeshRenderer>();
		_upper->SetMeshRenderer(MR);
		_upper->Start();
		_lower->SetMeshRenderer(MR);
		_lower->Start();
	}

	void Animator::Update()
	{
		_upper->Update();
		_lower->Update();
	}

	void Animator::SetUpperAnimationController(AnimationController* controller)
	{
		_upper->SetAnimationController(controller);
	}

	void Animator::SetLowerAnimationController(AnimationController* controller)
	{
		_lower->SetAnimationController(controller);
	}

	HDData::UpperAnimator* Animator::GetUpperAC()
	{
		return _upper;
	}

	LowerAnimator* Animator::GetLowerAC()
	{
		return _lower;
	}

}
