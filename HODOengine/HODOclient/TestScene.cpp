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

	API::LoadSceneFromData("ObjectInfo.json");

	auto mainCam = API::GetMainCamera()->GetGameObject();
	mainCam->AddComponent<CameraMove>();

// 	auto testBox1 = API::CreateObject(_scene);
// 	testBox1->GetComponent<HDData::Transform>()->SetPosition(1.f, 0.f, 0.f);
// 	auto boxRender1 = testBox1->AddComponent<HDData::MeshRenderer>();
// 	boxRender1->LoadMesh("Box01.fbx");
// 
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
	auto groundFloor = API::CreateObject(_scene);
	groundFloor->GetComponent<HDData::Transform>()->SetPosition(0.f, 0.f, 0.f);
	auto groundColli = groundFloor->AddComponent<HDData::StaticPlaneCollider>();

	// 플레이어 테스트
	auto playerTest = API::CreateObject(_scene, "player");
	playerTest->GetComponent<HDData::Transform>()->SetPosition(Vector3{ 0.f, 3.f, 0.f });
	playerTest->GetComponent<HDData::Transform>()->Rotate(0.f, 0.f, 0.f);
	playerTest->AddComponent<Player>();

	auto playerColli = playerTest->AddComponent<HDData::DynamicBoxCollider>();

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

	//auto playerMove = playerTest->AddComponent<PlayerMove>();
	//playerMove->SetPlayerCamera(_scene->GetMainCamera());
	//playerMove->SetPlayerText(playerPosText->GetComponent<HDData::TextUI>(), aimText->GetComponent<HDData::TextUI>(), hitText->GetComponent<HDData::TextUI>());
	//playerTest->AddComponent<PlayerMove>();
	//playerTest->GetComponent<PlayerMove>()->SetPlayerCamera(_scene->GetMainCamera());
	auto meshComp = playerTest->AddComponent<HDData::SkinnedMeshRenderer>();
	//auto meshComp = playerTest->AddComponent<HDData::MeshRenderer>();
	//meshComp->LoadMesh("A_TP_CH_Breathing.fbx");
	meshComp->LoadDiffuseMap("T_TP_CH_Basic_001_001_D.png");
	meshComp->PlayAnimation("A_TP_CH_Breathing.fbx", true);

	auto playerTestHead = API::CreateObject(_scene, "playerHead");	
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

