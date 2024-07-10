#include "TPScript.h"
#include "GameManager.h"
#include "PlayerMove.h"
#include "PlayerMoveState.h"
#include "RoundManager.h"

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
	if (!RoundManager::Instance()->GetIsRoundStart()) return;

	if (API::GetKeyPressing(DIK_W))
	{
		_animator->GetAllAC()->SetBool("isRunFront", true);

		if (API::GetKeyDown(DIK_LSHIFT))
		{
			if (GameManager::Instance()->GetMyObject()->GetComponent<PlayerMove>()->GetPlayerMoveEnum(1) != ePlayerMoveState::TUMBLE) return;
			_animator->GetAllAC()->SetTrigger("isRollFront");
		}
	}
	if (API::GetKeyUp(DIK_W))
	{
		_animator->GetAllAC()->SetBool("isRunFront", false);
	}


	if (API::GetKeyPressing(DIK_A))
	{
		_animator->GetAllAC()->SetBool("isRunLeft", true);

		if (API::GetKeyDown(DIK_LSHIFT))
		{
			if (GameManager::Instance()->GetMyObject()->GetComponent<PlayerMove>()->GetPlayerMoveEnum(1) != ePlayerMoveState::TUMBLE) return;
			_animator->GetAllAC()->SetTrigger("isRollLeft");
		}
	}
	if (API::GetKeyUp(DIK_A))
	{
		_animator->GetAllAC()->SetBool("isRunLeft", false);
	}


	if (API::GetKeyPressing(DIK_S))
	{
		_animator->GetAllAC()->SetBool("isRunBack", true);

		if (API::GetKeyDown(DIK_LSHIFT))
		{
			if (GameManager::Instance()->GetMyObject()->GetComponent<PlayerMove>()->GetPlayerMoveEnum(1) != ePlayerMoveState::TUMBLE) return;
			_animator->GetAllAC()->SetTrigger("isRollBack");
		}
	}
	if (API::GetKeyUp(DIK_S))
	{
		_animator->GetAllAC()->SetBool("isRunBack", false);
	}


	if (API::GetKeyPressing(DIK_D))
	{
		_animator->GetAllAC()->SetBool("isRunRight", true);

		if (API::GetKeyDown(DIK_LSHIFT))
		{
			if (GameManager::Instance()->GetMyObject()->GetComponent<PlayerMove>()->GetPlayerMoveEnum(1) != ePlayerMoveState::TUMBLE) return;
			_animator->GetAllAC()->SetTrigger("isRollRight");
		}
	}
	if (API::GetKeyUp(DIK_D))
	{
		_animator->GetAllAC()->SetBool("isRunRight", false);
	}


	if (API::GetKeyDown(DIK_SPACE))
	{
		_animator->GetAllAC()->SetTrigger("isJump");
	}

	if (API::GetKeyDown(DIK_R))
	{
		if (GameManager::Instance()->GetMyObject()->GetComponent<PlayerMove>()->GetPlayerMoveEnum(2) != ePlayerMoveState::RELOAD) return;
		_animator->GetAllAC()->SetTrigger("isReload");
	}

	if (GameManager::Instance()->GetMyInfo()->GetIsDie())
	{
		_animator->GetAllAC()->SetBool("isDie", true);
	}
	else
	{
		_animator->GetAllAC()->SetBool("isDie", false);
	}
}
