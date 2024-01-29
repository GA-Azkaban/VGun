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
	//playerTest->GetComponent<HDData::Transform>()->SetPosition(0.f, 0.f, 0.f);
	playerTest->AddComponent<Player>();
	playerTest->AddComponent<PlayerMove>();
	playerTest->GetComponent<PlayerMove>()->SetPlayerCamera(_scene->GetMainCamera());
	auto meshComp = playerTest->AddComponent<HDData::SkinnedMeshRenderer>();
	//auto meshComp = playerTest->AddComponent<HDData::MeshRenderer>();
	//meshComp->LoadMesh("Rob02.fbx");
	//meshComp->LoadDiffuseMap("Rob02Yellow_AlbedoTransparency.png");
	//meshComp->LoadNormalMap("Rob02White_Normal.png");
	meshComp->LoadMesh("A_TP_CH_Sprint_F.fbx");
	meshComp->LoadDiffuseMap("T_TP_CH_Basic_001_001_D.png");
	meshComp->PlayAnimation(0, true);
	playerTest->GetComponent<HDData::Transform>()->SetScale(Vector3{0.3f, 0.3f, 0.3f});
	playerTest->GetComponent<HDData::Transform>()->Rotate(-90.0f, 0.0f, 0.0f);
	//playerTest->GetComponent<HDData::Transform>()->SetRotation(Quaternion{0.6f, 0.0f, 0.0f, 0.0f});
	//playerTest->GetComponent<HDData::Transform>()->SetPosition(Vector3{1.f, 1.f, 1.f});
	auto playerColli = playerTest->AddComponent<HDData::DynamicBoxCollider>();
	//playerTest->AddComponent<HDData::StaticBoxCollider>();
	
	//auto playerHeadCollider = playerTest->AddComponent<HDData::DynamicBoxCollider>();
	
	/*auto playerTestHead = API::CreateObject(_scene, "playerHead");
	playerTestHead->AddComponent<HDData::MeshRenderer>();
	playerTestHead->SetParentObject(playerTest);
	playerTestHead->GetComponent<HDData::Transform>()->SetLocalPosition(Vector3{0.f, 1.1f, 0.f});

	auto textTest = API::CreateTextbox(_scene);
	textTest->GetTransform()->SetPosition({ 50.0f,50.0f,50.0f });
	textTest->GetComponent<HDData::TextUI>()->SetText("Seen yoon jae Ba bo");*/

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

	//auto button = API::CreateButton(_scene);
	//button->GetTransform()->SetPosition({ 50.0f,50.0f,50.0f });
	//button->GetComponent<HDData::Button>()->SetOnClickEvent([button]() {button->GetTransform()->Translate({ 1.0f,0.0f,0.0f }); });

	auto slider = API::CreateSlider(_scene, 50, "slider");
	slider->GetTransform()->SetPosition({ 500.0f,500.0f,0.0f });
	slider->AddComponent<HDData::AudioSource>();

	auto toggle = API::CreateToggle(_scene);
	toggle->GetTransform()->SetPosition({ 1500.f, 200.f, 0.0f });

	auto textInputBox = API::CreateTextInputBox(_scene);
	textInputBox->GetTransform()->SetPosition({1500.f, 500.f, 0});

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

