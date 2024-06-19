#include "TPScript.h"
#include "../HODOengine/HODO_API.h"

TPScript::TPScript()
{

}

void TPScript::Start()
{
	_animator = GetGameObject()->GetComponent<HDData::Animator>();

}

void TPScript::Update()
{
	if (API::GetKeyPressing(DIK_W))
	{
		_animator->GetAllAC()->SetBool("isWalkFront", true);
	}
	if (API::GetKeyUp(DIK_W))
	{
		_animator->GetAllAC()->SetBool("isWalkFront", false);
	}
	if (API::GetKeyPressing(DIK_A))
	{
		_animator->GetAllAC()->SetBool("isWalkLeft", true);
	}
	if (API::GetKeyUp(DIK_A))
	{
		_animator->GetAllAC()->SetBool("isWalkLeft", false);
	}
	if (API::GetKeyPressing(DIK_S))
	{
		_animator->GetAllAC()->SetBool("isWalkBack", true);
	}
	if (API::GetKeyUp(DIK_S))
	{
		_animator->GetAllAC()->SetBool("isWalkBack", false);

	}
	if (API::GetKeyPressing(DIK_D))
	{
		_animator->GetAllAC()->SetBool("isWalkRight", true);

	}
	if (API::GetKeyUp(DIK_D))
	{
		_animator->GetAllAC()->SetBool("isWalkRight", false);

	}

	if (API::GetKeyDown(DIK_SPACE))
	{
		_animator->GetAllAC()->SetTrigger("isJump");
	}
	if (API::GetKeyDown(DIK_R))
	{
		_animator->GetAllAC()->SetTrigger("isReload");
	}
	if (API::GetMouseDown(MOUSE_LEFT))
	{
		_animator->GetAllAC()->SetTrigger("isFire");
	}
}
