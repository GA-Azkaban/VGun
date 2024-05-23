#include "TrainingSceneView.h"
#include "CameraMove.h"
#include "FPAniScript.h"

TrainingSceneView::TrainingSceneView()
{
	_scene = API::CreateScene("Training");
}

TrainingSceneView::~TrainingSceneView()
{

}

void TrainingSceneView::Initialize()
{
	auto mainCam = _scene->GetMainCamera();
	mainCam->GetGameObject()->AddComponent<CameraMove>();

	HDData::GameObject* player = API::CreateObject(_scene, "player", mainCam->GetGameObject());
	player->LoadFBXFile("SKM_TP_X_Default.fbx");
	player->GetTransform()->SetLocalPosition(1, 1, 0);
	player->GetTransform()->Rotate(0, 180, 0);

	auto material = API::GetMaterial("TP_Red");
	auto meshComp = player->GetComponentInChildren<HDData::SkinnedMeshRenderer>();
	meshComp->LoadMaterial(material, 0);
	meshComp->LoadMaterial(material, 1);
	meshComp->LoadMaterial(material, 2);
	meshComp->LoadMaterial(material, 3);
	meshComp->LoadMaterial(material, 4);

	meshComp->SetMeshActive(false, 0);
	meshComp->SetMeshActive(false, 1);
	meshComp->SetMeshActive(false, 3);
	meshComp->SetMeshActive(false, 4);


	meshComp->PlayAnimation("AR_idle", true);

	player->AddComponent<HDData::Animator>();
	API::LoadFPAnimationFromData(player, "FP_animation.json");
	player->AddComponent<FPAniScript>();

	API::LoadSceneFromData("lobbyData.json", _scene);
}
