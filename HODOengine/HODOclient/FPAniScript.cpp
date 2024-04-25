#include "FPAniScript.h"

FPAniScript::FPAniScript()
{

}

void FPAniScript::Start()
{
	_animator = GetGameObject()->GetComponent<HDData::Animator>();
}

void FPAniScript::Update()
{
	if (API::GetKeyPressing(DIK_I))
	{
		_animator->GetAllAC()->SetBool("isWalk", true);
	}
	if (API::GetKeyUp(DIK_I))
	{
		_animator->GetAllAC()->SetBool("isWalk", false);
	}
	if (API::GetKeyPressing(DIK_SPACE))
	{
		_animator->GetAllAC()->SetBool("isShoot", true);
	}
	if (API::GetKeyUp(DIK_SPACE))
	{
		_animator->GetAllAC()->SetBool("isShoot", false);
	}
}
