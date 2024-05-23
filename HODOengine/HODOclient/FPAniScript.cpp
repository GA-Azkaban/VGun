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
			_animator->GetAllAC()->SetBool("isFire", true);
		}
	}
	if (API::GetMouseUp(MOUSE_LEFT))
	{
		_animator->GetAllAC()->SetBool("isFire", false);
	}
	if (API::GetKeyDown(DIK_R))
	{
		int bullet = GameManager::Instance()->GetMyInfo()->GetCurrentBulletCount();

		if (bullet != 30)
		{
			_animator->GetAllAC()->SetTrigger("isReload");
			GameManager::Instance()->GetMyInfo()->SetCurrentBulletCount(30);
		}
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
