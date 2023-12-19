#include "TestScene.h"
#include "../HODOengine/ObjectSystem.h"
#include "../HODOengine/GameObject.h"
#include "../HODOengine/Component.h"
#include "CameraMove.h"
#include "Player.h"
#include "PlayerMove.h"

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

	auto debugtest = API::CreateObject(_scene);
	debugtest->GetComponent<HDData::Transform>()->SetWorldScale(HDMath::HDFLOAT3{5.f, 5.f, 5.f});
	auto colli = debugtest->AddComponent<HDData::StaticBoxCollider>();
	colli->Setflag(eColliderType::PLAYER);


	// 플레이어 테스트
	auto playerTest = API::CreateObject(_scene);
	playerTest->AddComponent<Player>();
	playerTest->AddComponent<PlayerMove>();
	//auto playerColli = playerTest->AddComponent<HDData::>

	
	// 디버그 모드 활성화 (활성화하고 싶은 플래그를 인자로 넣음)
	API::DebugModeOn(eColliderType::PLAYER);

	API::LoadScene(_scene);
}

TestScene::~TestScene()
{

}

void TestScene::Start()
{
	
}