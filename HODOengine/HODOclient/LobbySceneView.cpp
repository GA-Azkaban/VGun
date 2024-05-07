#include "LobbySceneView.h"
#include "CameraMove.h"

LobbySceneView::LobbySceneView()
{

}

LobbySceneView::~LobbySceneView()
{

}

void LobbySceneView::Initialize()
{
	_scene = API::CreateScene("Lobby");

	auto mainCam = _scene->GetMainCamera();
	mainCam->GetGameObject()->AddComponent<CameraMove>();
	mainCam->GetGameObject()->GetTransform()->SetPosition(-21.31, 4.65, 0.807);
	mainCam->GetGameObject()->GetTransform()->Rotate(12, -102, 0);

	auto playerTest = API::CreateObject(_scene, "player");
	playerTest->GetComponent<HDData::Transform>()->SetPosition(Vector3{ -30.0f, 0.f, 0.0f });
	playerTest->GetTransform()->Rotate(0, -90, 0);
	playerTest->LoadFBXFile("SKM_TP_X_Default.fbx");

	auto meshComp = playerTest->GetComponentInChildren<HDData::SkinnedMeshRenderer>();
	HDEngine::MaterialDesc desc;
	desc.materialName = "TP_Red";
	desc.albedo = "TP_Red_B.png";
	HDData::Material* newMat = API::CreateMaterial(desc);
	meshComp->LoadMaterial(newMat, 0);
	meshComp->LoadMaterial(newMat, 1);
	meshComp->LoadMaterial(newMat, 2);
	meshComp->LoadMaterial(newMat, 3);
	meshComp->LoadMaterial(newMat, 4);

	meshComp->PlayAnimationUpper("AR_idle", true);
	meshComp->PlayAnimationLower("AR_idle", true);

	//API::LoadSceneFromData("lobbyData.json", _scene);
}
