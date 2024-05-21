#include "InGameSceneView.h"
#include "CameraMove.h"
#include "PlayerMove.h"
#include "FSMtestScript.h"

InGameSceneView::InGameSceneView()
{

}

InGameSceneView::~InGameSceneView()
{

}

void InGameSceneView::Initialize()
{
	_scene = API::CreateScene("InGame");

	auto mainCam = _scene->GetMainCamera();
	mainCam->GetGameObject()->AddComponent<CameraMove>();

	 //플레이어 테스트
	/*auto playerTest = API::CreateObject(_scene, "player");
	playerTest->GetComponent<HDData::Transform>()->SetPosition(Vector3{ 0.0f, 0.0f, 0.0f });
	playerTest->LoadFBXFile("SKM_TP_X_idle.fbx");

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

	playerTest->AddComponent<HDData::Animator>();
	API::LoadUpperAnimationFromData(playerTest, "upperdata.json");
	API::LoadLowerAnimationFromData(playerTest, "lowerdata.json");

	playerTest->AddComponent<FSMtestScript>();

	auto playerBodyCollider = playerTest->AddComponent<HDData::DynamicBoxCollider>(0.5f, 1.0f, 0.5f, 1);
	auto playerMove = playerTest->AddComponent<PlayerMove>();
	playerMove->SetPlayerCamera(_scene->GetMainCamera());

	auto playerPosText = API::CreateTextbox(_scene);
	playerPosText->GetTransform()->SetPosition(Vector3(1700.0f, 40.0f, 50.0f));
	playerPosText->GetComponent<HDData::TextUI>()->SetColor(DirectX::XMVECTOR{ 1.0f, 0.0f, 0.0f, 1.0f });
	auto aimText = API::CreateTextbox(_scene);
	aimText->GetTransform()->SetPosition(Vector3(950.0f, 520.0f, 50.0f));
	aimText->GetComponent<HDData::TextUI>()->SetColor(DirectX::XMVECTOR{ 1.0f, 0.0f, 0.0f, 1.0f });
	aimText->GetComponent<HDData::TextUI>()->SetText("");
	playerMove->SetPlayerText(playerPosText->GetComponent<HDData::TextUI>(), aimText->GetComponent<HDData::TextUI>());


	auto playerTestHead = playerTest->GetGameObjectByNameInChildren("head");
	playerTestHead->GetTransform()->SetLocalPosition(Vector3(0.0f, 1.0f, 0.0f));
	playerTestHead->SetParentObject(playerTest);
	auto playerHeadCollider = playerTestHead->AddComponent<HDData::DynamicSphereCollider>(0.35f, true);
	playerHeadCollider->SetParentCollider(playerBodyCollider);

	auto headCamObj = API::CreateObject(_scene, "headCamObj");
	headCamObj->SetParentObject(playerTestHead);
	headCamObj->GetTransform()->SetLocalPosition(Vector3{ 0.0f, 0.12f, 0.2f });
	auto headCam = headCamObj->AddComponent<HDData::Camera>();
	playerMove->SetHeadCam(headCam);

	auto plLeftUpperArm = playerTest->GetGameObjectByNameInChildren("upperarm_l");
	plLeftUpperArm->GetTransform()->SetLocalPosition(-1.0f, 0.0f, 0.0f);
	plLeftUpperArm->SetParentObject(playerTest);
	auto playerLUCollider = plLeftUpperArm->AddComponent<HDData::DynamicBoxCollider>(0.2f, 0.5f, 0.2f, 1);
	playerLUCollider->SetParentCollider(playerBodyCollider);*/

	API::LoadSceneFromData("sceneData.json", this->_scene);
}
