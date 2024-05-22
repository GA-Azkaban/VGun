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
		int bullet = GameManager::Instance()->GetMyInfo()->GetCurrentBulletCount();

		if (bullet <= 0)
		{
			_animator->GetAllAC()->SetTrigger("isEmpty");
		}
		else
		{
			//_animator->GetAllAC()->SetBool("isFire", true);
			//GetGameObject()->GetComponentInChildren<HDData::SkinnedMeshRenderer>()->PlayAnimation("AR_fire", true, 0.1, false, 0.01);
		}
	}
	//if (API::GetMouseUp(MOUSE_LEFT))
	//{
	//	_animator->GetAllAC()->SetBool("isFire", false);
	//}
	if (API::GetKeyDown(DIK_R))
	{
		//_animator->GetAllAC()->SetTrigger("isReload");
		GetGameObject()->GetComponentInChildren<HDData::SkinnedMeshRenderer>()->PlayAnimation("AR_reload");
	}


	// bullet
	if (API::GetKeyDown(DIK_F1))
	{
		GameManager::Instance()->GetMyInfo()->SetCurrentBulletCount(0);
	}
	if (API::GetKeyDown(DIK_F2))
	{
		GameManager::Instance()->GetMyInfo()->SetCurrentBulletCount(30);
	}
}
