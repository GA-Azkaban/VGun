﻿#include "TestScene.h"
#include "../HODOengine/ObjectSystem.h"
#include "../HODOengine/GameObject.h"
#include "../HODOengine/Component.h"
#include "../HODOengine/AudioSource.h"
#include "CameraMove.h"
#include "Player.h"
#include "PlayerMove.h"
#include "TestSound.h"
#include "SliderSoundScript.h"

enum eColliderType
{
	PLAYER = 1,
	BOX = 2
};

TestScene::TestScene()
{
	_scene = API::CreateScene("Test");
	//auto mainCam = _scene->GetMainCamera()->GetGameObject();
	auto mainCam = API::GetMainCamera()->GetGameObject();
	mainCam->AddComponent<CameraMove>();

	//auto camera = API::CreateObject(_scene);
	//camera->AddComponent<MovableCamera>()->SetMainCamera();
	//camera->GetTransform()->SetWorldPosition(Vector3{ 0.0f, 0.0f, -10.0f });

	//auto gameObject = hodoEngine::CreateObject(_scene);
	//auto comp = gameObject->AddComponent<hodoEngine::DebugCube>();
	////auto test = comp->GetGameObject();
	//comp->Get()->SetFillModeWireframe();
	//comp->Get()->SetColor(HDMaths::HDFLOAT4{ 1.0f, 0.0f, 0.0f, 0.0f });

	//auto debugtest = API::CreateObject(_scene);
	//debugtest->GetComponent<HDData::Transform>()->SetWorldScale(Vector3{5.f, 5.f, 5.f});
	//auto colli = debugtest->AddComponent<HDData::StaticBoxCollider>();
	//colli->Setflag(eColliderType::PLAYER);


	// floor as static plane
	auto groundFloor = API::CreateObject(_scene);
	groundFloor->GetComponent<HDData::Transform>()->SetPosition(0.f, 0.f, 0.f);
	auto groundColli = groundFloor->AddComponent<HDData::StaticPlaneCollider>();


	// 플레이어 테스트
	auto playerTest = API::CreateObject(_scene, "player");
	playerTest->GetComponent<HDData::Transform>()->SetPosition(0.f, 0.f, 0.f);
	playerTest->GetComponent<HDData::Transform>()->Rotate(0.f, 0.f, 0.f);
	playerTest->AddComponent<Player>();
	auto playerPosText = API::CreateTextbox(_scene);
	playerPosText->GetTransform()->SetPosition(Vector3(1700.0f, 40.0f, 50.0f));
	playerPosText->GetComponent<HDData::TextUI>()->SetColor(DirectX::XMVECTOR{1.0f, 0.0f, 0.0f, 1.0f});
	auto aimText = API::CreateTextbox(_scene);
	aimText->GetTransform()->SetPosition(Vector3(960.0f, 530.0f, 50.0f));
	aimText->GetComponent<HDData::TextUI>()->SetColor(DirectX::XMVECTOR{1.0f, 0.0f, 0.0f, 1.0f});
	aimText->GetComponent<HDData::TextUI>()->SetText("");
	auto hitText = API::CreateTextbox(_scene);
	hitText->GetTransform()->SetPosition(Vector3());
	hitText->GetComponent<HDData::TextUI>()->SetColor(DirectX::XMVECTOR{1.0f, 0.0f, 1.0f, 1.0f});
	hitText->GetComponent<HDData::TextUI>()->SetText("!!!!!!!!!!!!!!!!!");
	//playerTest->AddComponent<HDData::TextUI>();
	//playerTest->GetComponent<HDData::TextUI>()->SetText("x : ,  y: , z : ");
	//playerTest->GetComponent<HDData::TextUI>()->SetPosition(1200.0f, 1200.0f, 0.0f);
	auto playerMove = playerTest->AddComponent<PlayerMove>();
	playerMove->SetPlayerCamera(_scene->GetMainCamera());
	playerMove->SetPlayerText(playerPosText->GetComponent<HDData::TextUI>(), aimText->GetComponent<HDData::TextUI>(), hitText->GetComponent<HDData::TextUI>());
	auto meshComp = playerTest->AddComponent<HDData::SkinnedMeshRenderer>();
	meshComp->LoadMesh("Rob02.fbx");
	meshComp->LoadDiffuseMap("Rob02Yellow_AlbedoTransparency.png");
	meshComp->LoadNormalMap("Rob02White_Normal.png");
	meshComp->PlayAnimation(0, true);

	playerTest->GetComponent<HDData::Transform>()->SetPosition(Vector3{ 0.f, 3.f, 0.f });
	auto playerColli = playerTest->AddComponent<HDData::DynamicBoxCollider>();
	//playerTest->AddComponent<HDData::StaticBoxCollider>();

	//auto playerHeadCollider = playerTest->AddComponent<HDData::DynamicBoxCollider>();

	auto playerTestHead = API::CreateObject(_scene, "playerHead");
	//playerTestHead->AddComponent<HDData::MeshRenderer>();
	playerTestHead->SetParentObject(playerTest);
	playerTestHead->GetComponent<HDData::Transform>()->SetLocalPosition(Vector3{ 0.f, 1.1f, 0.f });
	auto headCam = playerTestHead->AddComponent<HDData::Camera>();
	playerMove->SetHeadCam(headCam);

	auto sphereTest = API::CreateObject(_scene, "sphereTest");
	sphereTest->GetComponent<HDData::Transform>()->SetPosition(-5.f, 3.f, 0.f);
	auto sphereCollider = sphereTest->AddComponent<HDData::DynamicSphereCollider>();


	auto textTest = API::CreateTextbox(_scene);
	textTest->GetTransform()->SetPosition({ 50.0f,50.0f,50.0f });
	textTest->GetComponent<HDData::TextUI>()->SetText("Seen yoon jae Ba bo");

	//HDData::GameObject* imageTest = API::CreateObject(_scene);
	//imageTest->GetTransform()->SetWorldPosition({ 50.0f,50.0f,50.0f });
	//HDData::ImageUI* imageComp = imageTest->AddComponent<HDData::ImageUI>();
	//imageComp->ChangeScale(0.5f, 0.5f);
	//imageComp->GetGameObject();
	//imageComp->SetColor(DirectX::Colors::AliceBlue);

	//auto UItest = API::CreateSlidebox(_scene);
	//UItest->GetTransform()->SetWorldPosition({1000.f, 100.f, 0.f});
	//auto sliderComp = UItest->GetComponent<HDData::SlideBoxUI>();
	//sliderComp->AddTextList("1");
	//sliderComp->AddTextList("2");
	//sliderComp->AddTextList("3");
	//sliderComp->AddTextList("4");

	//auto imageTest = API::CreateImageBox(_scene);
	//imageTest->GetTransform()->SetWorldPosition({ 1000.0f,1000.0f,50.0f });

	auto _button = API::CreateButton(_scene);
	_button->GetTransform()->SetPosition({ 50.0f,50.0f,50.0f });
	_button->GetComponent<HDData::Button>()->SetOnClickEvent([_button]() {_button->GetTransform()->Translate({ 1.0f,0.0f,0.0f }); });

	//auto _slider = API::CreateSlider(_scene, "slider");
	//_slider->GetTransform()->SetWorldPosition({ 500.0f,500.0f,0.0f });
	//_slider->AddComponent<HDData::AudioSource>();
	//_slider->AddComponent<SliderSoundScript>();

	//auto toggle = API::CreateToggle(_scene);
	//toggle->GetTransform()->SetWorldPosition({1500.f, 50.f, 0.0f});

	API::LoadScene(_scene);

	// 디버그 모드를 시작하는 함수
	API::DebugModeOn(eColliderType::PLAYER);
}

TestScene::~TestScene()
{

}

void TestScene::Start()
{

}

void TestScene::ClickEvent()
{
}

