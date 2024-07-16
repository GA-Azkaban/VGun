#include "FPAniScript.h"
#include "GameManager.h"
#include "PlayerMove.h"
#include "PlayerMoveState.h"
#include "NetworkManager.h"
#include "RoundManager.h"

FPAniScript::FPAniScript()
{

}

void FPAniScript::Start()
{
	_animator = GetGameObject()->GetComponent<HDData::Animator>();
}

void FPAniScript::Update()
{
	if (!RoundManager::Instance()->GetIsRoundStart()) return;

	// play animation in FP
	if (API::GetMouseDown(MOUSE_LEFT))
	{
		_animator->GetAllAC()->SetTrigger("isFire");
	}
	if (API::GetKeyDown(DIK_R))
	{
		if (GameManager::Instance()->GetMyObject()->GetComponent<PlayerMove>()->GetPlayerMoveEnum(2) != ePlayerMoveState::RELOAD) return;
		_animator->GetAllAC()->SetTrigger("isReload");
		_animator->GetAllAC()->SetBool("isIdle", false);
	}
	if (API::GetKeyDown(DIK_LSHIFT))
	{
		_animator->GetAllAC()->SetBool("isIdle", true);
	}
}
