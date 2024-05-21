#include "InGameSceneView.h"
#include "CameraMove.h"
#include "PlayerMove.h"
#include "FSMtestScript.h"
#include "RoundManager.h"

InGameSceneView::InGameSceneView()
{

}

InGameSceneView::~InGameSceneView()
{

}

void InGameSceneView::Initialize()
{
	_scene = API::CreateScene("InGame");

	auto skybox = API::CreateObject(_scene);
	auto skyboxComp = skybox->AddComponent<HDData::CubeMapRenderer>();
	skyboxComp->LoadCubeMapTexture("Day Sun Peak Clear.dds");

	auto mainCam = _scene->GetMainCamera();
	mainCam->GetGameObject()->AddComponent<CameraMove>();

	// 플레이어 여섯 명 렌더링

	float posX = 0;
	float posT = 165;

	HDEngine::MaterialDesc red;
	red.materialName = "TP_Red";
	red.albedo = "TP_Red_B.png";

	HDData::Material* M_Red = API::CreateMaterial(red);

	for (int i = 0; i < 6; ++i)
	{
		HDData::GameObject* player = API::CreateObject(_scene, "player");
		player->LoadFBXFile("SKM_TP_X_Default.fbx");
		player->GetTransform()->SetPosition(posX, 0, 0);

		auto meshComp = player->GetComponentInChildren<HDData::SkinnedMeshRenderer>();
		meshComp->LoadMaterial(M_Red, 0);
		meshComp->LoadMaterial(M_Red, 1);
		meshComp->LoadMaterial(M_Red, 2);
		meshComp->LoadMaterial(M_Red, 3);
		meshComp->LoadMaterial(M_Red, 4);

		meshComp->PlayAnimation("AR_idle", true);

		RoundManager::Instance()->GetPlayerObjs().push_back(player);

		player->SetSelfActive(false);

		posX += 1;
		posT += 315;
	}

	API::LoadSceneFromData("sceneData.json", this->_scene);
}
