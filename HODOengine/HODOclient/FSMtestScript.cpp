#include "FSMtestScript.h"



FSMtestScript::FSMtestScript()
{
}

void FSMtestScript::Start()
{
	_animator = GetGameObject()->GetComponent<HDData::Animator>();
}

void FSMtestScript::Update()
{
	if (API::GetKeyPressing(DIK_I))
	{
		_animator->SetBool("isWalk", true);
	}
	if (API::GetKeyUp(DIK_I))
	{
		_animator->SetBool("isWalk", false);
	}
	if (API::GetKeyDown(DIK_SPACE))
	{
		_animator->SetTrigger("isJump");
	}
}
