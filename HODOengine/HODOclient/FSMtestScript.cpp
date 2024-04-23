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
	if (API::GetKeyPressing(DIK_W))
	{
		_animator->SetBool("isRun", true);
	}
	if (API::GetKeyUp(DIK_W))
	{
		_animator->SetBool("isRun", false);
	}
	if (API::GetKeyDown(DIK_SPACE))
	{
		_animator->SetTrigger("isJump");
	}
	if (API::GetKeyDown(DIK_C))
	{
		if (_animator->GetCurrentState() == "IDLE" || _animator->GetCurrentState() == "IDLE_AIM" || _animator->GetCurrentState() == "CRUNCH_AIM")
		{
			_animator->SetBool("isCrouch", true);
		}
		else
		{
			_animator->SetBool("isCrouch", false);
		}
	}
}
