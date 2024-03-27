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

	auto mainCam = API::GetMainCamera()->GetGameObject();
	mainCam->AddComponent<CameraMove>();

	//auto skybox = API::CreateObject(_scene);
	//auto skyboxComp = skybox->AddComponent<HDData::CubeMapRenderer>();
	//skyboxComp->LoadCubeMapTexture("sunsetcube1024.dds");
	//skyboxComp->LoadCubeMapTexture("Day Sun Peak Clear.dds");
	//skyboxComp->LoadCubeMapTexture("skyboxmap.png");

	auto testBox1 = API::CreateObject(_scene);
	testBox1->GetComponent<HDData::Transform>()->SetPosition(0.0f, -0.5f, 0.0f);
	auto boxRender1 = testBox1->AddComponent<HDData::MeshRenderer>();
	boxRender1->LoadMesh("cube");
	boxRender1->SetMetallicValue(0.0f);
	
	auto testBox2 = API::CreateObject(_scene);
	testBox2->GetComponent<HDData::Transform>()->SetPosition(-10.0f, -0.5f, 0.0f);
	auto boxRender2 = testBox2->AddComponent<HDData::MeshRenderer>();
	boxRender2->LoadMesh("cube");
	boxRender2->SetMetallicValue(0.0f);
	
	auto testBox3 = API::CreateObject(_scene);
	testBox3->GetComponent<HDData::Transform>()->SetPosition(0.0f, -0.5f, 10.0f);
	auto boxRender3 = testBox3->AddComponent<HDData::MeshRenderer>();
	boxRender3->LoadMesh("cube");
	boxRender3->SetMetallicValue(0.0f);
	
	auto testBox4 = API::CreateObject(_scene);
	testBox4->GetComponent<HDData::Transform>()->SetPosition(-10.0f, -0.5f, 10.0f);
	auto boxRender4 = testBox4->AddComponent<HDData::MeshRenderer>();
	boxRender4->LoadMesh("cube");
	boxRender4->SetMetallicValue(0.0f);

	auto testBox5 = API::CreateObject(_scene);
	testBox5->GetComponent<HDData::Transform>()->SetPosition(-15.0f, 4.5f, 0.0f);
	testBox5->GetComponent<HDData::Transform>()->Rotate(90.0f, 90.0f, 0.0f);
	auto boxRender5 = testBox5->AddComponent<HDData::MeshRenderer>();
	boxRender5->LoadMesh("cube");
	boxRender5->SetMetallicValue(0.0f);

	auto building = API::CreateObject(_scene);
	building->GetComponent<HDData::Transform>()->SetPosition(-10.0f, 0.0f, 3.0f);
	building->GetComponent<HDData::Transform>()->Rotate(90.0f, 0.0f, 0.0f);
	auto meshRenderer = building->AddComponent<HDData::MeshRenderer>();
	meshRenderer->LoadMesh("watch_tower.fbx");
	meshRenderer->SetAlbedoColor(223, 194, 152);

	// PBR, IBL Test
	//auto sphereTest = API::CreateObject(_scene);
	//sphereTest->GetComponent<HDData::Transform>()->SetPosition(-5.0f, 0.0f, 0.0f);
	//auto sphereRenderer = sphereTest->AddComponent<HDData::MeshRenderer>();
	//sphereRenderer->LoadMesh("sphere");
	//sphereRenderer->SetMetallicValue(0.2f);
	//sphereRenderer->SetRoughnessValue(0.7f);
	//
	//auto sphereTest2 = API::CreateObject(_scene);
	//sphereTest2->GetComponent<HDData::Transform>()->SetPosition(0.0f, 0.0f, 0.0f);
	//auto sphereRenderer2 = sphereTest2->AddComponent<HDData::MeshRenderer>();
	//sphereRenderer2->LoadMesh("sphere");
	//sphereRenderer2->SetMetallicValue(0.5f);
	//sphereRenderer2->SetRoughnessValue(0.5f);
	//
	//auto sphereTest3 = API::CreateObject(_scene);
	//sphereTest3->GetComponent<HDData::Transform>()->SetPosition(5.0f, 0.0f, 0.0f);
	//auto sphereRenderer3 = sphereTest3->AddComponent<HDData::MeshRenderer>();
	//sphereRenderer3->LoadMesh("sphere");
	//sphereRenderer3->SetMetallicValue(0.9f);
	//sphereRenderer3->SetRoughnessValue(0.1f);

	// 플레이어 테스트
	auto playerTest = API::CreateObject(_scene, "player");
	playerTest->GetComponent<HDData::Transform>()->SetPosition(Vector3{ 3.f, 0.5f, 0.f });
	playerTest->GetComponent<HDData::Transform>()->Rotate(0.f, 0.f, 0.f);
	playerTest->AddComponent<Player>();

	//auto playerColli = playerTest->AddComponent<HDData::DynamicBoxCollider>();

	//auto playerPosText = API::CreateTextbox(_scene);
	//playerPosText->GetTransform()->SetPosition(Vector3(100.0f, 40.0f, 50.0f));
	//playerPosText->GetComponent<HDData::TextUI>()->SetColor(DirectX::XMVECTOR{1.0f, 0.0f, 0.0f, 1.0f});

	//auto aimText = API::CreateTextbox(_scene);
	//aimText->GetTransform()->SetPosition(Vector3(960.0f, 530.0f, 50.0f));
	//aimText->GetComponent<HDData::TextUI>()->SetColor(DirectX::XMVECTOR{1.0f, 0.0f, 0.0f, 1.0f});
	//aimText->GetComponent<HDData::TextUI>()->SetText("");

	//auto hitText = API::CreateTextbox(_scene);
	//hitText->GetTransform()->SetPosition(Vector3());
	//hitText->GetComponent<HDData::TextUI>()->SetColor(DirectX::XMVECTOR{1.0f, 0.0f, 1.0f, 1.0f});
	//hitText->GetComponent<HDData::TextUI>()->SetText("!!!!!");

	////auto centerText = API::CreateTextbox(_scene);
	////centerText->GetTransform()->SetPosition(Vector3(1000.0f,1000.0f,0.0f));
	////centerText->GetComponent<HDData::TextUI>()->SetColor(DirectX::Colors::Crimson);
	////centerText->GetComponent<HDData::TextUI>()->SetText("check drawing text into center position");

	//auto playerMove = playerTest->AddComponent<PlayerMove>();
	//playerMove->SetPlayerCamera(_scene->GetMainCamera());
	//playerMove->SetPlayerText(playerPosText->GetComponent<HDData::TextUI>(), aimText->GetComponent<HDData::TextUI>(), hitText->GetComponent<HDData::TextUI>());
	//playerTest->GetComponent<PlayerMove>()->SetPlayerCamera(_scene->GetMainCamera());
	
	auto meshComp = playerTest->AddComponent<HDData::SkinnedMeshRenderer>();
	meshComp->LoadMesh("A_TP_CH_Breathing.fbx");
	meshComp->LoadAlbedoMap("T_TP_CH_Basic_001_001_D.png");
	meshComp->PlayAnimation("A_TP_CH_Breathing.fbx", true);
	//meshComp->SetOutlineActive(true);

	auto playerTest2 = API::CreateObject(_scene, "player2");
	playerTest2->GetComponent<HDData::Transform>()->SetPosition(Vector3{ 0.f, 0.5f, 0.f });
	playerTest2->GetComponent<HDData::Transform>()->Rotate(0.f, 0.f, 0.f);
	auto meshComp2 = playerTest2->AddComponent<HDData::SkinnedMeshRenderer>();
	meshComp2->LoadMesh("SK_FP_CH_Default.fbx");
	meshComp2->PlayAnimation("A_TP_CH_Sprint_F.fbx", true);
	meshComp2->LoadAlbedoMap("T_TP_CH_Basic_001_001_D.png");
	//meshComp2->SetOutlineActive(true);

	auto playerTest3 = API::CreateObject(_scene, "player3");
	playerTest3->GetComponent<HDData::Transform>()->SetPosition(Vector3{ -5.f, 0.5f, 0.f });
	playerTest3->GetComponent<HDData::Transform>()->Rotate(0.f, 0.f, 0.f);
	auto meshComp3 = playerTest3->AddComponent<HDData::SkinnedMeshRenderer>();
	meshComp3->LoadMesh("SK_FP_CH_Default.fbx");
	meshComp3->PlayAnimation("A_TP_CH_Sprint_F.fbx", true);
	meshComp3->LoadAlbedoMap("T_TP_CH_Basic_001_001_D.png");
	//meshComp3->SetOutlineActive(true);

	//auto weaponTest = API::CreateObject(_scene, "weapon", playerTest2);
	//weaponTest->AddComponent<Weapon>();
	//auto weaponComp = weaponTest->AddComponent<HDData::MeshRenderer>();
	//weaponComp->LoadMesh("SK_AR_01.fbx");
	//weaponComp->LoadDiffuseMap("T_WEP_Camo_001_D.png");
	//weaponComp->LoadNormalMap("T_WEP_Camo_N.png");

	//auto playerTestHead = API::CreateObject(_scene, "playerHead");
	//playerTestHead->SetParentObject(playerTest);
	//playerTestHead->GetComponent<HDData::Transform>()->SetLocalPosition(Vector3{ 0.f, 1.1f, 0.f });
	////auto playerHeadCollider = playerTestHead->AddComponent<HDData::DynamicBoxCollider>();

	////auto headCam = playerTestHead->AddComponent<HDData::Camera>();
	////playerMove->SetHeadCam(headCam);

	////auto sphereTest = API::CreateObject(_scene, "sphereTest");
	////sphereTest->GetComponent<HDData::Transform>()->SetPosition(-5.f, 3.f, 0.f);
	////auto sphereCollider = sphereTest->AddComponent<HDData::DynamicSphereCollider>();

	////textTest->GetTransform()->SetPosition({ 50.0f,50.0f,50.0f });
	////auto textTest = API::CreateTextbox(_scene);
	////textTest->GetComponent<HDData::TextUI>()->SetText("Seen yoon jae Ba bo");

	////HDData::GameObject* imageTest = API::CreateObject(_scene);
	////imageTest->GetTransform()->SetWorldPosition({ 50.0f,50.0f,50.0f });
	////imageComp->ChangeScale(0.5f, 0.5f);
	////HDData::ImageUI* imageComp = imageTest->AddComponent<HDData::ImageUI>();
	////imageComp->GetGameObject();
	////imageComp->SetColor(DirectX::Colors::AliceBlue);

	////auto UItest = API::CreateSlidebox(_scene);
	////UItest->GetTransform()->SetWorldPosition({1000.f, 100.f, 0.f});
	////auto sliderComp = UItest->GetComponent<HDData::SlideBoxUI>();
	////sliderComp->AddTextList("1");
	////sliderComp->AddTextList("2");
	////sliderComp->AddTextList("3");
	////sliderComp->AddTextList("4");

	////auto imageTest = API::CreateImageBox(_scene);
	////imageTest->GetTransform()->SetWorldPosition({ 1000.0f,1000.0f,50.0f });

	////auto button = API::CreateButton(_scene);
	////button->GetTransform()->SetPosition({ 50.0f,50.0f,50.0f });
	////button->GetComponent<HDData::Button>()->SetOnClickEvent([button]() {button->GetTransform()->Translate({ 1.0f,0.0f,0.0f }); });

	////auto slider = API::CreateSlider(_scene, 50, "slider");
	////slider->GetTransform()->SetPosition({ 500.0f,500.0f,0.0f });

	////auto canvas = API::CreateImageBox(_scene, "canvas");
	////canvas->GetTransform()->SetPosition(1500.f, 200.f, 0.f);

	////auto img = API::CreateButton(_scene);
	////img->GetComponent<HDData::Button>()->SetImage("Sound.png");
	////img->GetTransform()->SetPosition({ 1500.f, 600.f, 0.0f });
	////img->GetComponent<HDData::Button>()->SetOnClickEvent(
	////	[]()
	////	{
	////
	////	}
	////);

	////auto img2 = API::CreateImageBox(_scene, "img2", img);
	////img2->GetComponent<HDData::ImageUI>()->SetImage("Mute.png");
	////img2->GetTransform()->SetPosition({ 1500.f, 500.f, 0.0f });

	////auto img3 = API::CreateImageBox(_scene, "img3", img2);
	////img3->GetComponent<HDData::ImageUI>()->SetImage("Mute.png");
	////img3->GetTransform()->SetPosition({ 1500.f, 600.f, 0.0f });

	////auto img4 = API::CreateImageBox(_scene, "img4", img3);
	////img4->GetComponent<HDData::ImageUI>()->SetImage("Mute.png");
	////img4->GetTransform()->SetPosition({ 1500.f, 700.f, 0.0f });

	////auto toggle = API::CreateToggle(_scene);
	////toggle->GetTransform()->SetPosition({ 1500.f, 200.f, 0.0f });
	////toggle->GetComponent<HDData::ToggleUI>()->SetSortOrder(0.21f);

	////auto textInputBox = API::CreateTextInputBox(_scene, "input", canvas);
	////textInputBox->GetTransform()->SetPosition({ 1500.f, 500.f, 0 });

	////canvas->SetSelfActive(true);

	API::LoadScene(_scene);

	//// 디버그 모드를 시작하는 함수
	//API::DebugModeOn(eColliderType::PLAYER);
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

