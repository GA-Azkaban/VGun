#include "TestScene.h"
#include "../HODOengine/ObjectSystem.h"
#include "../HODOengine/GameObject.h"
#include "../HODOengine/Component.h"
#include "../HODOengine/AudioSource.h"
#include "CameraMove.h"
#include "Player.h"
#include "PlayerMove.h"
#include "TestSound.h"

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
	auto playerTest = API::CreateObject(_scene);
	playerTest->GetComponent<HDData::Transform>()->SetWorldPosition(0.f, 0.f, 0.f);
	playerTest->AddComponent<Player>();
	playerTest->AddComponent<PlayerMove>();
	playerTest->AddComponent<HDData::MeshRenderer>();

	playerTest->AddComponent<HDData::AudioSource>();
	playerTest->AddComponent<TestSound>();
	//playerTest->AddComponent<HDData::StaticBoxCollider>();

	

	//auto playerColli = playerTest->AddComponent<HDData::DynamicBoxCollider>();
	//playerColli->Setflag(eColliderType::PLAYER);

	// collider 여러 개를 만들어 보자.
	auto boxTest = API::CreateObject(_scene);
	boxTest->GetComponent<HDData::Transform>()->SetWorldPosition(HDMath::HDFLOAT3{5.f, 10.f, 1.f});
	//auto boxCollider = boxTest->AddComponent<HDData::DynamicBoxCollider>();

	auto sphereTest = API::CreateObject(_scene);
	sphereTest->GetComponent<HDData::Transform>()->SetWorldPosition(HDMath::HDFLOAT3{-5.f, 10.f, 1.f});
	//auto sphereCollider = sphereTest->AddComponent<HDData::DynamicSphereCollider>();

	auto textTest = API::CreateTextbox(_scene);
	textTest->GetTransform()->SetWorldPosition({ 50.0f,50.0f,50.0f });
	textTest->GetComponent<HDData::TextUI>()->SetText("Seen yoon jae Ba bo");

<<<<<<< HEAD
	HDData::GameObject * imageTest = API::CreateObject(_scene);
	imageTest->GetTransform()->SetWorldPosition({ 50.0f,50.0f,50.0f });
	HDData::ImageUI* imageComp = imageTest->AddComponent<HDData::ImageUI>();
	imageComp->ChangeScale(0.5f, 0.5f);
	imageComp->GetGameObject();

=======
	auto UItest = API::CreateSlidebox(_scene);
	UItest->GetTransform()->SetWorldPosition({1000.f, 100.f, 0.f});
	auto sliderComp = UItest->GetComponent<HDData::SlideBoxUI>();
	sliderComp->AddTextList("1");
	sliderComp->AddTextList("2");
	sliderComp->AddTextList("3");
	sliderComp->AddTextList("4");

	/*auto imageTest = API::CreateImageBox(_scene);
	imageTest->GetTransform()->SetWorldPosition({ 50.0f,50.0f,50.0f });*/

	/*auto _button = API::CreateButton(_scene);
	_button->GetTransform()->SetWorldPosition({ 50.0f,50.0f,50.0f });
	_button->GetComponent<HDData::Button>()->SetOnClickEvent([_button]() {_button->GetTransform()->Translate({ 1.0f,0.0f,0.0f }); });*/

	auto _slider = API::CreateSlider(_scene);
	_slider->GetTransform()->SetWorldPosition({ 50.0f,50.0f,50.0f });
>>>>>>> ab573bef3ee680cc888d7abe33ca669fae428aea

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

