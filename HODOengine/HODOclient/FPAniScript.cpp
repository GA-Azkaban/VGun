#include "FPAniScript.h"
#include "GameManager.h"

FPAniScript::FPAniScript()
{

}

void FPAniScript::Start()
{
	_animator = GetGameObject()->GetComponent<HDData::Animator>();
}

void FPAniScript::Update()
{
	if (API::GetMouseHold(MOUSE_LEFT))
	{
		//GetGameObject()->GetComponentInChildren<HDData::SkinnedMeshRenderer>()->PlayAnimation("AR_fire", false, true, 0.1);
		_animator->GetAllAC()->SetTrigger("isFire");
	}
	if (API::GetKeyDown(DIK_R))
	{
		_animator->GetAllAC()->SetTrigger("isReload");
	}

	// set character die
	if (API::GetKeyDown(DIK_F1))
	{
		_animator->GetAllAC()->SetBool("isDie", true);
		//GetGameObject()->GetComponentInChildren<HDData::SkinnedMeshRenderer>()->PlayAnimationAtOnce("AR_dying");
	}
}
