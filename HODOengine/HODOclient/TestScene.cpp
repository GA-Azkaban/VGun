#include "TestScene.h"
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
	auto mainCam = _scene->GetMainCamera()->GetGameObject();
	mainCam->AddComponent<CameraMove>();

	//auto camera = API::CreateObject(_scene);
	//camera->AddComponent<MovableCamera>()->SetMainCamera();
	//camera->GetTransform()->SetWorldPosition(HDMath::HDFLOAT3{ 0.0f, 0.0f, -10.0f });

	//auto gameObject = hodoEngine::CreateObject(_scene);
	//auto comp = gameObject->AddComponent<hodoEngine::DebugCube>();
	////auto test = comp->GetGameObject();
	//comp->Get()->SetFillModeWireframe();
	//comp->Get()->SetColor(HDMaths::HDFLOAT4{ 1.0f, 0.0f, 0.0f, 0.0f });

	//auto debugtest = API::CreateObject(_scene);
	//debugtest->GetComponent<HDData::Transform>()->SetWorldScale(HDMath::HDFLOAT3{5.f, 5.f, 5.f});
	//auto colli = debugtest->AddComponent<HDData::StaticBoxCollider>();
	//colli->Setflag(eColliderType::PLAYER);

	// 플레이어 테스트
	auto playerTest = API::CreateObject(_scene, "player");
	playerTest->GetComponent<HDData::Transform>()->SetWorldPosition(0.f, 0.f, 0.f);
	playerTest->AddComponent<Player>();
	playerTest->AddComponent<PlayerMove>();
	auto meshComp = playerTest->AddComponent<HDData::SkinnedMeshRenderer>();
	meshComp->LoadMesh("Rob02.fbx");
	meshComp->LoadDiffuseMap("Rob02Yellow_AlbedoTransparency.png");
	meshComp->LoadNormalMap("Rob02White_Normal.png");
	meshComp->PlayAnimation(0, true);
	playerTest->GetComponent<HDData::Transform>()->SetWorldPosition(HDMath::HDFLOAT3{1.f, 1.f, 1.f});
	auto playerColli = playerTest->AddComponent<HDData::DynamicBoxCollider>();
	//playerTest->AddComponent<HDData::StaticBoxCollider>();
	
	//auto playerHeadCollider = playerTest->AddComponent<HDData::DynamicBoxCollider>();
	
	auto playerTestHead = API::CreateObject(_scene, "playerHead");
	playerTestHead->AddComponent<HDData::MeshRenderer>();
	playerTestHead->SetParentObject(playerTest);
	playerTestHead->GetComponent<HDData::Transform>()->SetLocalPosition(HDMath::HDFLOAT3{0.f, 1.1f, 0.f});


	/*
	// collider 여러 개를 만들어 보자.
	auto boxTest = API::CreateObject(_scene);
	boxTest->GetComponent<HDData::Transform>()->SetWorldPosition(HDMath::HDFLOAT3{5.f, 5.f, 1.f});
	boxTest->AddComponent<HDData::MeshRenderer>();
	//auto boxCollider = boxTest->AddComponent<HDData::DynamicBoxCollider>();

	auto boxTest2 = API::CreateObject(_scene);
	boxTest2->GetComponent<HDData::Transform>()->SetWorldPosition(HDMath::HDFLOAT3{-3.f, 6.f, 0.f});
	boxTest2->AddComponent<HDData::MeshRenderer>();
	auto boxCollider2 = boxTest2->AddComponent<HDData::DynamicBoxCollider>();

	auto boxTest3 = API::CreateObject(_scene);
	boxTest3->GetComponent<HDData::Transform>()->SetWorldPosition(HDMath::HDFLOAT3{5.f, 7.f, 0.5f});
	boxTest3->AddComponent<HDData::MeshRenderer>();
	auto boxCollider3 = boxTest3->AddComponent<HDData::DynamicBoxCollider>();

	auto boxTest4 = API::CreateObject(_scene);
	boxTest4->GetComponent<HDData::Transform>()->SetWorldPosition(HDMath::HDFLOAT3{3.f, 8.f, -1.f});
	boxTest4->AddComponent<HDData::MeshRenderer>();
	auto boxCollider4 = boxTest4->AddComponent<HDData::DynamicBoxCollider>();

	auto boxTest5 = API::CreateObject(_scene);
	boxTest5->GetComponent<HDData::Transform>()->SetWorldPosition(HDMath::HDFLOAT3{-2.f, 9.f, -2.f});
	boxTest5->AddComponent<HDData::MeshRenderer>();
	auto boxCollider5 = boxTest5->AddComponent<HDData::DynamicBoxCollider>();

	for (int i = 0; i < 10; ++i)
	{
		auto boxbox = API::CreateObject(_scene);
		boxbox->GetComponent<HDData::Transform>()->SetWorldPosition(HDMath::HDFLOAT3{-7.f + 1.5f * i, 20.f + 5 * i, -7.f + 1.5f * i});
		boxbox->AddComponent<HDData::MeshRenderer>();
		auto boxboxCol = boxbox->AddComponent<HDData::DynamicBoxCollider>();
	}
	*/

	//auto sphereTest = API::CreateObject(_scene);
	//sphereTest->GetComponent<HDData::Transform>()->SetWorldPosition(HDMath::HDFLOAT3{-5.f, 10.f, 1.f});
	//auto sphereCollider = sphereTest->AddComponent<HDData::DynamicSphereCollider>();

	//auto textTest = API::CreateTextbox(_scene);
	//textTest->GetTransform()->SetWorldPosition({ 50.0f,50.0f,50.0f });
	//textTest->GetComponent<HDData::TextUI>()->SetText("Seen yoon jae Ba bo");

	//HDData::GameObject * imageTest = API::CreateObject(_scene);
	//imageTest->GetTransform()->SetWorldPosition({ 50.0f,50.0f,50.0f });
	//HDData::ImageUI* imageComp = imageTest->AddComponent<HDData::ImageUI>();
	//imageComp->ChangeScale(0.5f, 0.5f);
	//imageComp->GetGameObject();

	//auto UItest = API::CreateSlidebox(_scene);
	//UItest->GetTransform()->SetWorldPosition({1000.f, 100.f, 0.f});
	//auto sliderComp = UItest->GetComponent<HDData::SlideBoxUI>();
	//sliderComp->AddTextList("1");
	//sliderComp->AddTextList("2");
	//sliderComp->AddTextList("3");
	//sliderComp->AddTextList("4");

	//auto imageTest = API::CreateImageBox(_scene);
	//imageTest->GetTransform()->SetWorldPosition({ 1000.0f,1000.0f,50.0f });

	//auto _button = API::CreateButton(_scene);
	//_button->GetTransform()->SetWorldPosition({ 50.0f,50.0f,50.0f });
	//_button->GetComponent<HDData::Button>()->SetOnClickEvent([_button]() {_button->GetTransform()->Translate({ 1.0f,0.0f,0.0f }); });

	auto _slider = API::CreateSlider(_scene, "slider");
	_slider->GetTransform()->SetWorldPosition({ 500.0f,500.0f,0.0f });
	_slider->AddComponent<HDData::AudioSource>();
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

