#include "FPAniScript.h"
#include "GameManager.h"

FPAniScript::FPAniScript()
{

}

void FPAniScript::Start()
{
	_animator = GetGameObject()->GetComponent<HDData::Animator>();
}

void FPAniScript::Update()
{
	if (API::GetMouseHold(MOUSE_LEFT))
	{
		//GetGameObject()->GetComponentInChildren<HDData::SkinnedMeshRenderer>()->PlayAnimation("AR_fire", false, 0.1, true, 0.1);
		_animator->GetAllAC()->SetTrigger("isFire");
	}
	//if (API::GetMouseUp(MOUSE_LEFT))
	//{
	//	_animator->GetAllAC()->SetBool("isFire", false);
	//}
	if (API::GetKeyDown(DIK_R))
	{
		_animator->GetAllAC()->SetTrigger("isReload");
	}


	// bullet
	if (API::GetKeyDown(DIK_F1))
	{
		GameManager::Instance()->GetMyInfo()->SetCurrentBulletCount(10);
	}
	if (API::GetKeyDown(DIK_F2))
	{
		GameManager::Instance()->GetMyInfo()->SetCurrentBulletCount(30);
	}
}
