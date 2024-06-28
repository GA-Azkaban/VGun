#include "FPAniScript.h"
#include "GameManager.h"
#include "NetworkManager.h"

FPAniScript::FPAniScript()
{

}

void FPAniScript::Start()
{
	_animator = GetGameObject()->GetComponent<HDData::Animator>();
	//_mesh = GetGameObject()->GetGameObjectByNameInChildren("FPMesh")->GetComponentInChildren<HDData::SkinnedMeshRenderer>();
	//_info = GetGameObject()->GetComponent<PlayerInfo>();
}

void FPAniScript::Update()
{
	// play animation in FP
	if (API::GetMouseDown(MOUSE_LEFT))
	{
		_animator->GetAllAC()->SetTrigger("isFire");
	}
	if (API::GetKeyDown(DIK_R))
	{
		//TODO) 재장전 시 메시 잠깐 끄기
		//_mesh->SetMeshActive(false, 0);
		//_mesh->SetMeshActive(false, 1);
		//_mesh->SetMeshActive(false, 2);
		//_mesh->SetMeshActive(false, 3);
		//_mesh->SetMeshActive(false, 4);

		_animator->GetAllAC()->SetTrigger("isReload");
	}
	if (API::GetKeyDown(DIK_LCONTROL))
	{
		// roll
		// NetworkManager::Instance().SendPlayRoll();
	}



	////die
	//if (GetGameObject()->GetComponent<PlayerInfo>()->GetIsDie())
	//{
	//	_animator->GetAllAC()->SetBool("isDie", true);
	//}


}
