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
		_animator->GetUpperAC()->SetBool("isRun", true);
		_animator->GetLowerAC()->SetBool("isRun", true);
	}
	if (API::GetKeyUp(DIK_W))
	{
		_animator->GetUpperAC()->SetBool("isRun", false);
		_animator->GetLowerAC()->SetBool("isRun", false);
	}
	if (API::GetKeyDown(DIK_SPACE))
	{
		_animator->GetUpperAC()->SetTrigger("isJump");
		_animator->GetLowerAC()->SetTrigger("isJump");
	}
	if (API::GetKeyDown(DIK_C))
	{
		if (_animator->GetUpperAC()->GetCurrentState() == "IDLE" || _animator->GetUpperAC()->GetCurrentState() == "IDLE_AIM" || _animator->GetUpperAC()->GetCurrentState() == "CRUNCH_AIM")
		{
			_animator->GetUpperAC()->SetBool("isCrouch", true);
			_animator->GetLowerAC()->SetBool("isCrouch", true);
		}
		else
		{
			_animator->GetUpperAC()->SetBool("isCrouch", false);
			_animator->GetLowerAC()->SetBool("isCrouch", false);
		}
	}
	if (API::GetMouseDown(MOUSE_LEFT))
	{
		_animator->GetUpperAC()->SetTrigger("isShoot");
	}
}
