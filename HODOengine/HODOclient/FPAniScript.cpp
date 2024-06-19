#include "FPAniScript.h"
#include "GameManager.h"
#include "NetworkManager.h"

FPAniScript::FPAniScript()
{

}

void FPAniScript::Start()
{
	_animator = GetGameObject()->GetComponent<HDData::Animator>();
	//_info = GetGameObject()->GetComponent<PlayerInfo>();
}

void FPAniScript::Update()
{
	// play animation in FP
	if (API::GetMouseHold(MOUSE_LEFT))
	{
		_animator->GetAllAC()->SetTrigger("isFire");
	}
	if (API::GetKeyDown(DIK_R))
	{
		_animator->GetAllAC()->SetTrigger("isReload");
	}
	if (API::GetKeyDown(DIK_LSHIFT))
	{
		// roll
		//NetworkManager::Instance().SendPlayRoll();
	}



	// die
	//if (GetGameObject()->GetComponent<PlayerInfo>()->GetIsDie())
	//{
	//	_animator->GetAllAC()->SetBool("isDie", true);
	//}


}
