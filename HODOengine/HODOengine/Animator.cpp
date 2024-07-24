#include "Animator.h"
#include "GameObject.h"
#include "SkinnedMeshRenderer.h"

namespace HDData
{
	Animator::Animator()
	{
		_all = new AllAnimator();
	}

	Animator::~Animator()
	{
		delete _all;
	}

	void Animator::Start()
	{
		auto MR = GetGameObject()->GetComponentInChildren<HDData::SkinnedMeshRenderer>();
		_all->SetMeshRenderer(MR);
		_all->Start();
	}

	void Animator::Update()
	{
		_all->Update();
	}

	void Animator::SetAllAnimationController(AnimationController* controller)
	{
		_all->SetAnimationController(controller);
	}

	HDData::AllAnimator* Animator::GetAllAC()
	{
		return _all;
	}

}
