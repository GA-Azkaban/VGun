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
	if (API::GetKeyPressing(DIK_SPACE))
	{
		_animator->SetBool("isWalk", true);
	}
	if (API::GetKeyUp(DIK_SPACE))
	{
		_animator->SetBool("isWalk", false);
	}
}
