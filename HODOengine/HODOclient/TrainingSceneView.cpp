#include "TrainingSceneView.h"
#include "CameraMove.h"
#include "FPAniScript.h"
#include "BtnHoveringScript.h"

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

	// setting Canvas
	HDData::GameObject* preferencesBtn = API::CreateButton(_scene, "preferencesBtn");
	preferencesBtn->GetTransform()->SetPosition(1875.f, 30.f, 0.f);
	preferencesBtn->AddComponent<BtnHoveringScript>();
	auto prefBtn = preferencesBtn->GetComponent<HDData::Button>();
	prefBtn->SetImage("icon_cog.png");
	prefBtn->SetSortOrder(0.6f);
	prefBtn->SetOnClickEvent(
		[]() {

		});

	HDData::GameObject* settingControlObject = API::CreateImageBox(_scene, "settingControlObject");
	settingControlObject->GetTransform()->SetPosition(-500.0f, -500.0f, 0.0f);
	settingControlObject->SetSelfActive(false);

	HDData::GameObject* preferencesCanvas = API::CreateImageBox(_scene, "Setting", settingControlObject);
	preferencesCanvas->GetTransform()->SetPosition(960.f, 540.f, 0.f);
	preferencesCanvas->GetComponent<HDData::ImageUI>()->SetImage("alphaRefCanvas2.png");
	preferencesCanvas->GetComponent<HDData::ImageUI>()->SetSortOrder(0.6f);
	preferencesCanvas->GetComponent<HDData::ImageUI>()->SetIsIgnoreFocus(true);



	HDData::GameObject* player = API::CreateObject(_scene, "player", mainCam->GetGameObject());
	player->LoadFBXFile("SKM_TP_X_Default.fbx");
	player->GetTransform()->SetPosition(0, 0.3, -9);

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

	// print player info
	auto playernameindex = API::CreateTextbox(_scene, "playerNickname");
	playernameindex->GetTransform()->SetPosition(100, 800, 0);
	playernameindex->GetComponent<HDData::TextUI>()->SetText("Nickname");
	
	auto playerHealthindex = API::CreateTextbox(_scene, "playerHealth");
	playerHealthindex->GetTransform()->SetPosition(100, 850, 0);
	playerHealthindex->GetComponent<HDData::TextUI>()->SetText("Health");

	auto playerBulletCountindex = API::CreateTextbox(_scene, "playerBulletCount");
	playerBulletCountindex->GetTransform()->SetPosition(100, 900, 0);
	playerBulletCountindex->GetComponent<HDData::TextUI>()->SetText("Bullet");


	auto playername = API::CreateTextbox(_scene, "playerNickname");
	playername->GetTransform()->SetPosition(200, 800, 0);

	auto playerHealth = API::CreateTextbox(_scene, "playerHealth");
	playerHealth->GetTransform()->SetPosition(200, 850, 0);

	auto playerBulletCount = API::CreateTextbox(_scene, "playerBulletCount");
	playerBulletCount->GetTransform()->SetPosition(200, 900, 0);

	API::LoadSceneFromData("lobbyData.json", _scene);
}
