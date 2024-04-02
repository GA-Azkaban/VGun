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

	//auto testBox1 = API::CreateObject(_scene);
	////testBox1->GetComponent<HDData::Transform>()->SetPosition(0.0f, -0.5f, 0.0f);
	//testBox1->GetComponent<HDData::Transform>()->SetPosition(-3.0f, 0.0f, 0.0f);
	//auto boxRender1 = testBox1->AddComponent<HDData::MeshRenderer>();
	//boxRender1->LoadMesh("Plane.fbx");
	//boxRender1->SetMetallicValue(0.0f);
	//
	//auto testBox2 = API::CreateObject(_scene);
	//testBox2->GetComponent<HDData::Transform>()->SetPosition(-10.0f, -0.5f, 0.0f);
	//auto boxRender2 = testBox2->AddComponent<HDData::MeshRenderer>();
	//boxRender2->LoadMesh("cube");
	//boxRender2->SetMetallicValue(0.0f);
	//
	//auto testBox3 = API::CreateObject(_scene);
	//testBox3->GetComponent<HDData::Transform>()->SetPosition(0.0f, -0.5f, 10.0f);
	//auto boxRender3 = testBox3->AddComponent<HDData::MeshRenderer>();
	//boxRender3->LoadMesh("cube");
	//boxRender3->SetMetallicValue(0.0f);
	//
	//auto testBox4 = API::CreateObject(_scene);
	//testBox4->GetComponent<HDData::Transform>()->SetPosition(-10.0f, -0.5f, 10.0f);
	//auto boxRender4 = testBox4->AddComponent<HDData::MeshRenderer>();
	//boxRender4->LoadMesh("cube");
	//boxRender4->SetMetallicValue(0.0f);

	//auto testBox5 = API::CreateObject(_scene);
	//testBox5->GetComponent<HDData::Transform>()->SetPosition(-15.0f, 4.5f, 0.0f);
	//testBox5->GetComponent<HDData::Transform>()->Rotate(90.0f, 90.0f, 0.0f);
	//auto boxRender5 = testBox5->AddComponent<HDData::MeshRenderer>();
	//boxRender5->LoadMesh("cube");
	//boxRender5->SetMetallicValue(0.0f);
	//
	//auto testBox6 = API::CreateObject(_scene);
	//testBox6->GetComponent<HDData::Transform>()->SetPosition(-15.0f, 4.5f, 10.0f);
	//testBox6->GetComponent<HDData::Transform>()->Rotate(90.0f, 90.0f, 0.0f);
	//auto boxRender6 = testBox6->AddComponent<HDData::MeshRenderer>();
	//boxRender6->LoadMesh("cube");
	//boxRender6->SetMetallicValue(0.0f);

	//auto testBox5 = API::CreateObject(_scene);
	//testBox5->GetComponent<HDData::Transform>()->SetPosition(-0.0f, 4.5f, 15.0f);
	//testBox5->GetComponent<HDData::Transform>()->Rotate(90.0f, 0.0f, 0.0f);
	//auto boxRender5 = testBox5->AddComponent<HDData::MeshRenderer>();
	//boxRender5->LoadMesh("cube");
	//boxRender5->SetMetallicValue(0.0f);
	//
	//auto testBox6 = API::CreateObject(_scene);
	//testBox6->GetComponent<HDData::Transform>()->SetPosition(-10.0f, 4.5f, 15.0f);
	//testBox6->GetComponent<HDData::Transform>()->Rotate(90.0f, 0.0f, 0.0f);
	//auto boxRender6 = testBox6->AddComponent<HDData::MeshRenderer>();
	//boxRender6->LoadMesh("cube");
	//boxRender6->SetMetallicValue(0.0f);

	//auto building = API::CreateObject(_scene);
	//building->GetComponent<HDData::Transform>()->SetPosition(-10.0f, 0.0f, 8.0f);
	//building->GetComponent<HDData::Transform>()->Rotate(90.0f, 0.0f, 0.0f);
	//auto meshRenderer = building->AddComponent<HDData::MeshRenderer>();
	//meshRenderer->LoadMesh("watch_tower.fbx");
	//meshRenderer->SetAlbedoColor(223, 194, 152);

	//auto building2 = API::CreateObject(_scene);
	//building2->GetComponent<HDData::Transform>()->SetPosition(-5.0f, 0.45f, 5.0f);
	//building2->GetComponent<HDData::Transform>()->Rotate(0.0f, 180.0f, 90.0f);
	//auto meshRenderer2 = building2->AddComponent<HDData::MeshRenderer>();
	//meshRenderer2->LoadMesh("SM_House_Large 1.fbx");
	//meshRenderer2->SetAlbedoColor(223, 194, 152);

	// 플레이어 테스트
	auto playerTest = API::CreateObject(_scene, "player");
	playerTest->GetComponent<HDData::Transform>()->SetPosition(Vector3{ -5.0f, 0.5f, 0.0f });
	playerTest->AddComponent<Player>();
	playerTest->LoadNodeFromFBXFile("SKM_TP_X_Breathing.fbx");

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
	
	//auto meshComp = playerTest->AddComponent<HDData::SkinnedMeshRenderer>();
	auto meshComp = playerTest->GetComponentInChildren<HDData::SkinnedMeshRenderer>();
	meshComp->LoadAlbedoMap("T_TP_CH_Basic_001_001_D.png");
	meshComp->PlayAnimation("X_Breathing", true);
	//meshComp->SetOutlineActive(true);

	//auto playerTest2 = API::CreateObject(_scene, "player2");
	//playerTest2->GetComponent<HDData::Transform>()->SetPosition(Vector3{ -9.f, 0.5f, 7.f });
	//playerTest2->GetComponent<HDData::Transform>()->Rotate(0.0f, 45.0f, 0.0f);
	//auto meshComp2 = playerTest2->AddComponent<HDData::SkinnedMeshRenderer>();
	//meshComp2->LoadMesh("SK_FP_CH_Default.fbx");
	//meshComp2->PlayAnimation("A_TP_CH_Sprint_F.fbx", true);
	//meshComp2->LoadAlbedoMap("T_TP_CH_Basic_001_001_D.png");
	//meshComp2->SetOutlineActive(true);
	auto hand = playerTest->GetGameObjectByNameInChildren("ik_hand_r");
	auto weaponTest = API::CreateObject(_scene, "weapon", hand);
	weaponTest->GetComponent<HDData::Transform>()->Rotate(206.0f, 0.0f, 15.0f);
	auto weaponComp = weaponTest->AddComponent<HDData::MeshRenderer>();
	weaponComp->LoadMesh("SM_AR3.fbx");
	weaponComp->LoadAlbedoMap("T_WEP_Camo_001_D.png");
	weaponComp->LoadNormalMap("T_WEP_Camo_N.png");

	//HDData::GameObject* mainmenuCanvas = API::CreateImageBox(_scene, "mainmenuCanvas");
	//mainmenuCanvas->GetComponent<HDData::ImageUI>()->SetImage("test.jpg");
	//mainmenuCanvas->GetComponent<HDData::ImageUI>()->SetSortOrder(0.0f);
	//mainmenuCanvas->GetTransform()->SetPosition(960.f, 540.f, 0.f);

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

