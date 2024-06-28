#include "TPScript.h"

TPScript::TPScript()
{

}

void TPScript::Start()
{
	_animator = GetGameObject()->AddComponent<HDData::Animator>();
	API::LoadFPAnimationFromData(GetGameObject(), "TP_animation.json");
}

void TPScript::Update()
{

	if (API::GetMouseDown(MOUSE_LEFT))
	{
		_animator->GetAllAC()->SetTrigger("isFire");
	}

	if (API::GetKeyPressing(DIK_W))
	{
		_animator->GetAllAC()->SetBool("isRunFront", true);
	}
	if (API::GetKeyUp(DIK_W))
	{
		_animator->GetAllAC()->SetBool("isRunFront", false);
	}

	if (API::GetKeyPressing(DIK_A))
	{
		_animator->GetAllAC()->SetBool("isRunLeft", true);
	}
	if (API::GetKeyUp(DIK_A))
	{
		_animator->GetAllAC()->SetBool("isRunLeft", false);
	}

	if (API::GetKeyPressing(DIK_S))
	{
		_animator->GetAllAC()->SetBool("isRunBack", true);
	}
	if (API::GetKeyUp(DIK_S))
	{
		_animator->GetAllAC()->SetBool("isRunBack", false);
	}

	if (API::GetKeyPressing(DIK_D))
	{
		_animator->GetAllAC()->SetBool("isRunRight", true);
	}
	if (API::GetKeyUp(DIK_D))
	{
		_animator->GetAllAC()->SetBool("isRunRight", false);
	}

	if (API::GetKeyDown(DIK_R))
	{
		_animator->GetAllAC()->SetTrigger("isReload");
	}

	if (API::GetKeyDown(DIK_SPACE))
	{
		_animator->GetAllAC()->SetTrigger("isJump");
	}
}
