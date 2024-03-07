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
	API::LoadScene(_scene);

	// API::LoadSceneFromData("ObjectInfo.json");

	auto mainCam = API::GetMainCamera()->GetGameObject();
	mainCam->AddComponent<CameraMove>();

	auto skybox = API::CreateObject(_scene);
	auto skyboxComp = skybox->AddComponent<HDData::CubeMapRenderer>();
	skyboxComp->LoadCubeMapTexture("Day Sun Peak Clear.dds");
	//skyboxComp->LoadCubeMapTexture("sunsetcube1024.dds");
	//skyboxComp->LoadCubeMapTexture("skyboxmap.png");

 	auto testBox1 = API::CreateObject(_scene);
 	testBox1->GetComponent<HDData::Transform>()->SetPosition(0.0f, -0.5f, 0.0f);
 	auto boxRender1 = testBox1->AddComponent<HDData::MeshRenderer>();
 	boxRender1->LoadMesh("cube");
 	//boxRender1->LoadMesh("Box01.fbx");
	//boxRender1->LoadDiffuseMap("Default.png");
	auto testBox2 = API::CreateObject(_scene);
	testBox2->GetComponent<HDData::Transform>()->SetPosition(-5.0f, 5.0f, 0.0f);
	testBox2->GetComponent<HDData::Transform>()->Rotate(90.0f, 90.0f, 0.0f);
	auto boxRender2 = testBox2->AddComponent<HDData::MeshRenderer>();
	boxRender2->LoadMesh("cube");
 
// 	auto testBox2 = API::CreateObject(_scene);
// 	testBox2->GetComponent<HDData::Transform>()->SetPosition(-1.f, 0.f, 0.f);
// 	auto boxRender2 = testBox2->AddComponent<HDData::MeshRenderer>();
// 	boxRender2->LoadMesh("Box01.fbx");

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
	//auto groundFloor = API::CreateObject(_scene);
	//groundFloor->GetComponent<HDData::Transform>()->SetPosition(0.f, 0.f, 0.f);
	//auto groundColli = groundFloor->AddComponent<HDData::StaticPlaneCollider>();

	//// 플레이어 테스트
	auto playerTest = API::CreateObject(_scene, "player");
	playerTest->GetComponent<HDData::Transform>()->SetPosition(Vector3{ 3.f, 0.f, 0.f });
	playerTest->GetComponent<HDData::Transform>()->Rotate(0.f, 0.f, 0.f);
	playerTest->AddComponent<Player>();

	auto playerColli = playerTest->AddComponent<HDData::DynamicBoxCollider>();

	auto playerPosText = API::CreateTextbox(_scene);
	playerPosText->GetTransform()->SetPosition(Vector3(100.0f, 40.0f, 50.0f));
	playerPosText->GetComponent<HDData::TextUI>()->SetColor(DirectX::XMVECTOR{1.0f, 0.0f, 0.0f, 1.0f});

	auto aimText = API::CreateTextbox(_scene);
	aimText->GetTransform()->SetPosition(Vector3(960.0f, 530.0f, 50.0f));
	aimText->GetComponent<HDData::TextUI>()->SetColor(DirectX::XMVECTOR{1.0f, 0.0f, 0.0f, 1.0f});
	aimText->GetComponent<HDData::TextUI>()->SetText("");

	auto hitText = API::CreateTextbox(_scene);
	hitText->GetTransform()->SetPosition(Vector3());
	hitText->GetComponent<HDData::TextUI>()->SetColor(DirectX::XMVECTOR{1.0f, 0.0f, 1.0f, 1.0f});
	hitText->GetComponent<HDData::TextUI>()->SetText("!!!!!");

	//auto centerText = API::CreateTextbox(_scene);
	//centerText->GetTransform()->SetPosition(Vector3(1000.0f,1000.0f,0.0f));
	//centerText->GetComponent<HDData::TextUI>()->SetColor(DirectX::Colors::Crimson);
	//centerText->GetComponent<HDData::TextUI>()->SetText("check drawing text into center position");

	//auto playerMove = playerTest->AddComponent<PlayerMove>();
	//playerMove->SetPlayerCamera(_scene->GetMainCamera());
	//playerMove->SetPlayerText(playerPosText->GetComponent<HDData::TextUI>(), aimText->GetComponent<HDData::TextUI>(), hitText->GetComponent<HDData::TextUI>());
	//playerTest->GetComponent<PlayerMove>()->SetPlayerCamera(_scene->GetMainCamera());
	auto meshComp = playerTest->AddComponent<HDData::SkinnedMeshRenderer>();
	//auto meshComp = playerTest->AddComponent<HDData::SkinnedMeshRenderer>();
	//auto meshComp = playerTest->AddComponent<HDData::MeshRenderer>();
	//meshComp->LoadMesh("A_TP_CH_Breathing.fbx");
	//meshComp->LoadDiffuseMap("T_TP_CH_Basic_001_001_D.png");
	//meshComp->PlayAnimation("A_TP_CH_Breathing.fbx", true);


	auto playerTestHead = API::CreateObject(_scene, "playerHead");
	playerTestHead->SetParentObject(playerTest);
	playerTestHead->GetComponent<HDData::Transform>()->SetLocalPosition(Vector3{ 0.f, 1.1f, 0.f });
	//auto playerHeadCollider = playerTestHead->AddComponent<HDData::DynamicBoxCollider>();

	//auto headCam = playerTestHead->AddComponent<HDData::Camera>();
	//playerMove->SetHeadCam(headCam);

	//auto sphereTest = API::CreateObject(_scene, "sphereTest");
	//sphereTest->GetComponent<HDData::Transform>()->SetPosition(-5.f, 3.f, 0.f);
	//auto sphereCollider = sphereTest->AddComponent<HDData::DynamicSphereCollider>();

	//textTest->GetTransform()->SetPosition({ 50.0f,50.0f,50.0f });
	//auto textTest = API::CreateTextbox(_scene);
	//textTest->GetComponent<HDData::TextUI>()->SetText("Seen yoon jae Ba bo");

	//HDData::GameObject* imageTest = API::CreateObject(_scene);
	//imageTest->GetTransform()->SetWorldPosition({ 50.0f,50.0f,50.0f });
	//imageComp->ChangeScale(0.5f, 0.5f);
	//HDData::ImageUI* imageComp = imageTest->AddComponent<HDData::ImageUI>();
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

	//auto slider = API::CreateSlider(_scene, 50, "slider");
	//slider->GetTransform()->SetPosition({ 500.0f,500.0f,0.0f });

	auto canvas = API::CreateImageBox(_scene, "canvas");
	canvas->GetTransform()->SetPosition(1500.f, 200.f, 0.f);

	auto img = API::CreateButton(_scene);
	img->GetComponent<HDData::Button>()->SetImage("Sound.png");
	img->GetTransform()->SetPosition({ 1500.f, 600.f, 0.0f });
	img->GetComponent<HDData::Button>()->SetOnClickEvent(
		[]()
		{

		}
	);

	//auto img2 = API::CreateImageBox(_scene, "img2", img);
	//img2->GetComponent<HDData::ImageUI>()->SetImage("Mute.png");
	//img2->GetTransform()->SetPosition({ 1500.f, 500.f, 0.0f });

	//auto img3 = API::CreateImageBox(_scene, "img3", img2);
	//img3->GetComponent<HDData::ImageUI>()->SetImage("Mute.png");
	//img3->GetTransform()->SetPosition({ 1500.f, 600.f, 0.0f });

	//auto img4 = API::CreateImageBox(_scene, "img4", img3);
	//img4->GetComponent<HDData::ImageUI>()->SetImage("Mute.png");
	//img4->GetTransform()->SetPosition({ 1500.f, 700.f, 0.0f });

	//auto toggle = API::CreateToggle(_scene);
	//toggle->GetTransform()->SetPosition({ 1500.f, 200.f, 0.0f });
	//toggle->GetComponent<HDData::ToggleUI>()->SetSortOrder(0.21f);

	//auto textInputBox = API::CreateTextInputBox(_scene, "input", canvas);
	//textInputBox->GetTransform()->SetPosition({ 1500.f, 500.f, 0 });

	canvas->SetSelfActive(true);

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

