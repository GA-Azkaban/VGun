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
		_animator->SetBool("isWalk", true);
	}
	if (API::GetKeyUp(DIK_W))
	{
		_animator->SetBool("isWalk", false);
	}
	if (API::GetKeyDown(DIK_SPACE))
	{
		_animator->SetTrigger("isJump");
	}
	if (API::GetKeyDown(DIK_C))
	{
		if (_animator->GetCurrentState() == "IDLE" || _animator->GetCurrentState() == "IDLE_AIM" || _animator->GetCurrentState() == "CRUNCH_AIM")
		{
			_animator->SetBool("isCrunch", true);
		}
		else
		{
			_animator->SetBool("isCrunch", false);
		}
	}
	if (API::GetKeyDown(DIK_F))
	{
		if (_animator->GetCurrentState() == "IDLE" || _animator->GetCurrentState() == "CRUNCH")
		{
			_animator->SetBool("isAim", true);
		}
		else
		{
			_animator->SetBool("isAim", false);
		}
	}
	if (API::GetMouseHold(MOUSE_LEFT))
	{
		if (_animator->GetCurrentState() == "IDLE_AIM")
		{
			_animator->SetTrigger("isShoot");
		}
		else if (_animator->GetCurrentState() == "CRUNCH")
		{
			_animator->SetTrigger("isCrunchShoot");
		}
	}
}
